#include "tuner.h"
#include "config.h"
#include "threadpool.h"
#include "TuneEval.h"
#include "base.h"
#include "bitboard_masks.h"
#include "evaluation.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;
using namespace Tuner;

#ifndef TAPERED
static_assert(false, "Tuner requires TAPERED to be defined")
#endif
static_assert(
    TuneEval::l2_regularization > 0,
    "A positive L2 value is required for a strongly convex objective");
static_assert(
    TuneEval::lbfgs_history_size > 0
        && TuneEval::validation_interval > 0
        && TuneEval::early_stopping_patience > 0,
    "Optimizer intervals and history sizes must be positive");

struct WdlMarker
{
    string marker;
    tune_t wdl;
};

struct CoefficientEntry
{
    int16_t value;
    int16_t index;
};

struct Entry
{
    vector<CoefficientEntry> coefficients;
    tune_t wdl;
    bool white_to_move;
    tune_t additional_score;
    uint64_t split_key;
#if TAPERED
    int32_t phase;
    tune_t endgame_scale;
#endif
};

static const array<WdlMarker, 7> markers
{
    WdlMarker{"1.0", 1},
    WdlMarker{"1-0", 1},
    WdlMarker{"1/2-1/2", 0.5},
    WdlMarker{"0-1", 0},
    WdlMarker{"|White",1},
	WdlMarker{"|Black",0},
	WdlMarker{"|Draw",0.5}
};

static tune_t get_fen_wdl(const string& original_fen, const bool original_white_to_move, const bool white_to_move, const bool side_to_move_wdl)
{
    tune_t wdl;
    bool marker_found = false;
    for (auto& marker : markers)
    {
        if (original_fen.find(marker.marker) != std::string::npos)
        {
            if (marker_found)
            {
                cout << "WDL marker already found on line " << original_fen << endl;
                throw std::runtime_error("WDL marker already found");
            }
            marker_found = true;
            wdl = marker.wdl;
        }
    }

    if(!marker_found)
    {
        stringstream ss(original_fen);
        while (!ss.eof())
        {
            string word;
            ss >> word;
            if (word.starts_with("0."))
            {
                wdl = stod(word);
                marker_found = true;
            }
            else if (word.starts_with("[0."))
            {
                wdl = stod(word.substr(1, word.size() - 2));
                marker_found = true;
            }
        }
    }

    if (!marker_found)
    {
        cout << "WDL marker not found on line " << original_fen << endl;
        throw std::runtime_error("WDL marker not found");
    }

    if(!original_white_to_move && side_to_move_wdl)
    {
        wdl = 1 - wdl;
    }

    return wdl;
}   

static bool get_fen_color_to_move(const string& fen)
{
    return fen.find('w') != std::string::npos;
}

static void print_elapsed(high_resolution_clock::time_point start)
{
    const auto now = high_resolution_clock::now();
    const auto elapsed = now - start;
    const auto elapsed_seconds = duration_cast<seconds>(elapsed).count();
    cout << "[" << elapsed_seconds << "s] ";
}

static void get_coefficient_entries(const coefficients_t& coefficients, vector<CoefficientEntry>& coefficient_entries, int32_t parameter_count)
{
    if(coefficients.size() != parameter_count)
    {
        throw runtime_error("Parameter count mismatch");
    }

    for (int16_t i = 0; i < coefficients.size(); i++)
    {
        if (coefficients[i] == 0)
        {
            continue;
        }

        const auto coefficient_entry = CoefficientEntry{coefficients[i], i};
        coefficient_entries.push_back(coefficient_entry);
    }
}

static tune_t linear_eval(const Entry& entry, const parameters_t& parameters)
{
    tune_t score = entry.additional_score;
#if TAPERED 
    tune_t midgame = 0;
    tune_t endgame = 0;
    for (const auto& coefficient : entry.coefficients)
    {
        midgame += coefficient.value * parameters[coefficient.index][static_cast<int32_t>(PhaseStages::Midgame)];
        endgame += coefficient.value * parameters[coefficient.index][static_cast<int32_t>(PhaseStages::Endgame)] * entry.endgame_scale;
    }
    score += (midgame * entry.phase + endgame * (24 - entry.phase)) / 24;
#else
    for (const auto& coefficient : entry.coefficients)
    {
        score += coefficient.value * parameters[coefficient.index];
    }
#endif

    return score;
}

static int32_t get_phase(const string& fen)
{
    int32_t phase = 0;
    auto stop = false;
    for(const char ch : fen)
    {
        if(stop) break;

        switch (ch)
        {
        case 'n': case 'N':
        case 'b': case 'B':
            phase += 1;
            break;
        case 'r': case 'R':
            phase += 2;
            break;
        case 'q': case 'Q':
            phase += 4;
            break;
        case ' ':
            stop = true;
            break;
        }
    }
    return phase;
}

static void print_statistics(const parameters_t& parameters, const vector<Entry>& entries)
{
    array<size_t, 2> wins{};
    array<size_t, 2> draws{};
    array<size_t, 2> losses{};
    array<size_t, 2> total{};
    array<tune_t, 2> wdls{};

    size_t min_parameters = std::numeric_limits<uint64_t>::max();
    size_t max_parameters = 0;
    size_t total_parameters = 0;

    for(auto& entry : entries)
    {
        if(entry.wdl == 1)
            wins[entry.white_to_move]++;
        else if(entry.wdl == 0.5)
            draws[entry.white_to_move]++;
        else if (entry.wdl == 0.0)
            losses[entry.white_to_move]++;
        
        total[entry.white_to_move]++;
        wdls[entry.white_to_move] += entry.wdl;

        if(entry.coefficients.size() < min_parameters)
            min_parameters = entry.coefficients.size();
        if (entry.coefficients.size() > max_parameters)
            max_parameters = entry.coefficients.size();

        total_parameters += entry.coefficients.size();
    }

    cout << "Dataset statistics:" << endl;
    cout << "Total positions: " << entries.size() << endl;
    for(int color = 1; color >= 0; color--)
    {
        const auto color_name = color ? "White" : "Black";
        cout << color_name << ": " << total[color] << " (" << (total[color] * 100.0 / entries.size()) << "%)" << endl;
        cout << color_name << " 1.0: " << wins[color] << " (" << (wins[color] * 100.0 / entries.size()) << "%)" << endl;
        cout << color_name << " 0.5: " << draws[color] << " (" << (draws[color] * 100.0 / entries.size()) << "%)" << endl;
        cout << color_name << " 0.0: " << losses[color] << " (" << (losses[color] * 100.0 / entries.size()) << "%)" << endl;
        cout << color_name << " avg: " << wdls[color] / total[color] << endl;
    }

    auto avg_parameters = static_cast<tune_t>(total_parameters) / entries.size();
    cout << "Parameters total: " << parameters.size() << endl;
    cout << "Parameters min: " << min_parameters << endl;
    cout << "Parameters max: " << max_parameters << endl;
    cout << "Parameters avg: " << avg_parameters << endl;
    cout << endl;
}


static string get_parameter_name(int index)
{
    switch (index)
    {
    case PAWN: return "PAWN";
    case KNIGHT: return "KNIGHT";
    case BISHOP: return "BISHOP";
    case ROOK: return "ROOK";
    case QUEEN: return "QUEEN";
    case FORWARD_BLOCKED_BACKWARD: return "FORWARD_BLOCKED_BACKWARD";
    case FORWARD_CONTROLLED_BACKWARD: return "FORWARD_CONTROLLED_BACKWARD";
    case FREE_TO_ADV_BACKWARD: return "FREE_TO_ADV_BACKWARD";
    case KING_FILE_OPEN_WITH_RQ: return "KING_FILE_OPEN_WITH_RQ";
    case KING_FILE_OPEN_WITHOUT_RQ: return "KING_FILE_OPEN_WITHOUT_RQ";
    case ADJ_FILE_OPEN_WITH_RQ: return "ADJ_FILE_OPEN_WITH_RQ";
    case ADJ_FILE_OPEN_WITHOUT_RQ: return "ADJ_FILE_OPEN_WITHOUT_RQ";
    case NO_DEF_PAWNS_ON_KING_FILE_WITH_RQ: return "NO_DEF_PAWNS_ON_KING_FILE_WITH_RQ";
    case NO_DEF_PAWNS_ON_KING_FILE_WITHOUT_RQ: return "NO_DEF_PAWNS_ON_KING_FILE_WITHOUT_RQ";
    case NO_ATT_PAWNS_ON_KING_FILE_WITH_RQ: return "NO_ATT_PAWNS_ON_KING_FILE_WITH_RQ";
    case NO_ATT_PAWNS_ON_KING_FILE_WITHOUT_RQ: return "NO_ATT_PAWNS_ON_KING_FILE_WITHOUT_RQ";
    case FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITH_RQ: return "FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITH_RQ";
    case FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITHOUT_RQ: return "FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITHOUT_RQ";
    case FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITH_RQ: return "FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITH_RQ";
    case FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITHOUT_RQ: return "FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITHOUT_RQ";
    case RIGHT_DEF_QUEEN_SIDE_SQUARE_WITH_QB: return "RIGHT_DEF_QUEEN_SIDE_SQUARE_WITH_QB";
    case RIGHT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB: return "RIGHT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB";
    case LEFT_DEF_QUEEN_SIDE_SQUARE_WITH_QB: return "LEFT_DEF_QUEEN_SIDE_SQUARE_WITH_QB";
    case LEFT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB: return "LEFT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB";
    case RIGHT_DEF_KING_SIDE_SQUARE_WITH_QB: return "RIGHT_DEF_KING_SIDE_SQUARE_WITH_QB";
    case RIGHT_DEF_KING_SIDE_SQUARE_WITHOUT_QB: return "RIGHT_DEF_KING_SIDE_SQUARE_WITHOUT_QB";
    case LEFT_DEF_KING_SIDE_SQUARE_WITH_QB: return "LEFT_DEF_KING_SIDE_SQUARE_WITH_QB";
    case LEFT_DEF_KING_SIDE_SQUARE_WITHOUT_QB: return "LEFT_DEF_KING_SIDE_SQUARE_WITHOUT_QB";
    case RIGHT_DEF_CENTRAL_SQUARE_WITH_QB: return "RIGHT_DEF_CENTRAL_SQUARE_WITH_QB";
    case RIGHT_DEF_CENTRAL_SQUARE_WITHOUT_QB: return "RIGHT_DEF_CENTRAL_SQUARE_WITHOUT_QB";
    case LEFT_DEF_CENTRAL_SQUARE_WITH_QB: return "LEFT_DEF_CENTRAL_SQUARE_WITH_QB";
    case LEFT_DEF_CENTRAL_SQUARE_WITHOUT_QB: return "LEFT_DEF_CENTRAL_SQUARE_WITHOUT_QB";
    case QUEEN_SIDE_FORWARD_SQUARE: return "QUEEN_SIDE_FORWARD_SQUARE";
    case CENTRAL_FORWARD_SQUARE: return "CENTRAL_FORWARD_SQUARE";
    case KING_SIDE_FORWARD_SQUARE: return "KING_SIDE_FORWARD_SQUARE";
    case WEAK_PAWNS_AROUND_SMALL_KING: return "WEAK_PAWNS_AROUND_SMALL_KING";
    case WEAK_PAWNS_AROUND_BIG_KING: return "WEAK_PAWNS_AROUND_BIG_KING";
    case OP_PAWNS_IN_SMALL_MASK: return "OP_PAWNS_IN_SMALL_MASK";
    case OP_PAWNS_IN_BIG_MASK: return "OP_PAWNS_IN_BIG_MASK";
    case MOBILITY_PENALTY: return "MOBILITY_PENALTY";
    case ROOK_ON_OPEN_FILE: return "ROOK_ON_OPEN_FILE";
    case ROOK_ON_SEMI_OPEN_FILE: return "ROOK_ON_SEMI_OPEN_FILE";
    case CONNECTED_ROOKS: return "CONNECTED_ROOKS";
    case BISHOP_PAIR: return "BISHOP_PAIR";
    case BAD_BISHOP_BLOCKED: return "BAD_BISHOP_BLOCKED";
    case BAD_BISHOP_UNBLOCKED: return "BAD_BISHOP_UNBLOCKED";
    case TRAPPED_BISHOP: return "TRAPPED_BISHOP";
    case TRAPPED_KNIGHT: return "TRAPPED_KNIGHT";
    case FIANCHETTO_BISHOP: return "FIANCHETTO_BISHOP";
    case BROKEN_FIANCHETTO: return "BROKEN_FIANCHETTO";
    case BISHOP_OUTPOST_NO_OPPOSITE_BISHOP: return "BISHOP_OUTPOST_NO_OPPOSITE_BISHOP";
    case BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP: return "BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP";
    case KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP: return "KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP";
    case KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP: return "KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP";
    }

    if (index >= PAWN_PST_START && index <= PAWN_PST_END) return "PAWN_PST_START+" + std::to_string(index - PAWN_PST_START);
    if (index >= KNIGHT_PST_START && index <= KNIGHT_PST_END) return "KNIGHT_PST_START+" + std::to_string(index - KNIGHT_PST_START);
    if (index >= BISHOP_PST_START && index <= BISHOP_PST_END) return "BISHOP_PST_START+" + std::to_string(index - BISHOP_PST_START);
    if (index >= ROOK_PST_START && index <= ROOK_PST_END) return "ROOK_PST_START+" + std::to_string(index - ROOK_PST_START);
    if (index >= QUEEN_PST_START && index <= QUEEN_PST_END) return "QUEEN_PST_START+" + std::to_string(index - QUEEN_PST_START);
    if (index >= KING_PST_START && index <= KING_PST_END) return "KING_PST_START+" + std::to_string(index - KING_PST_START);
    if (index >= PASSED_PAWNS_START && index <= PASSED_PAWNS_END) return "PASSED_PAWNS_START+" + std::to_string(index - PASSED_PAWNS_START);
    if (index >= PROTECTED_PASSED_PAWNS_START && index <= PROTECTED_PASSED_PAWNS_END) return "PROTECTED_PASSED_PAWNS_START+" + std::to_string(index - PROTECTED_PASSED_PAWNS_START);
    if (index >= BLOCKED_FREE_PAWN_START && index <= BLOCKED_FREE_PAWN_END) return "BLOCKED_FREE_PAWN_START+" + std::to_string(index - BLOCKED_FREE_PAWN_START);
    if (index >= CANT_REACHED_BY_ENEMY_KING_START && index <= CANT_REACHED_BY_ENEMY_KING_END) return "CANT_REACHED_BY_ENEMY_KING_START+" + std::to_string(index - CANT_REACHED_BY_ENEMY_KING_START);
    if (index >= OWN_KING_IS_CLOSE_START && index <= OWN_KING_IS_CLOSE_END) return "OWN_KING_IS_CLOSE_START+" + std::to_string(index - OWN_KING_IS_CLOSE_START);
    if (index >= OWN_KING_IS_FAR_START && index <= OWN_KING_IS_FAR_END) return "OWN_KING_IS_FAR_START+" + std::to_string(index - OWN_KING_IS_FAR_START);
    if (index >= ROOK_BEHIND_FREE_PAWN_START && index <= ROOK_BEHIND_FREE_PAWN_END) return "ROOK_BEHIND_FREE_PAWN_START+" + std::to_string(index - ROOK_BEHIND_FREE_PAWN_START);
    if (index >= OP_ROOK_BEHIND_FREE_PAWN_START && index <= OP_ROOK_BEHIND_FREE_PAWN_END) return "OP_ROOK_BEHIND_FREE_PAWN_START+" + std::to_string(index - OP_ROOK_BEHIND_FREE_PAWN_START);
    if (index >= ISOLANI_START && index <= ISOLANI_END) return "ISOLANI_START+" + std::to_string(index - ISOLANI_START);
    if (index >= BLOCKED_ISOLANI_START && index <= BLOCKED_ISOLANI_END) return "BLOCKED_ISOLANI_START+" + std::to_string(index - BLOCKED_ISOLANI_START);
    if (index >= PROTECTED_ISOLANI_START && index <= PROTECTED_ISOLANI_END) return "PROTECTED_ISOLANI_START+" + std::to_string(index - PROTECTED_ISOLANI_START);
    if (index >= DOUBLE_PAWN_FILE_START && index <= DOUBLE_PAWN_FILE_END) return "DOUBLE_PAWN_FILE_START+" + std::to_string(index - DOUBLE_PAWN_FILE_START);
    if (index > PAWNS_DEFEND_DIAGONAL_START && index <= PAWNS_DEFEND_DIAGONAL_END) return "PAWNS_DEFEND_DIAGONAL_START+" + std::to_string(index - PAWNS_DEFEND_DIAGONAL_START);
    if (index >= OTHER_DEF_COUNT_NO_PAWNS_START && index <= OTHER_DEF_COUNT_NO_PAWNS_END) return "OTHER_DEF_COUNT_NO_PAWNS_START+" + std::to_string(index - OTHER_DEF_COUNT_NO_PAWNS_START);
    if (index >= OTHER_DEF_COUNT_WITH_PAWNS_START && index <= OTHER_DEF_COUNT_WITH_PAWNS_END) return "OTHER_DEF_COUNT_WITH_PAWNS_START+" + std::to_string(index - OTHER_DEF_COUNT_WITH_PAWNS_START);
    if (index >= SMALL_ATTACK_COUNT_START && index <= SMALL_ATTACK_COUNT_END) return "SMALL_ATTACK_COUNT_START+" + std::to_string(index - SMALL_ATTACK_COUNT_START);
    if (index >= BIG_ATTACK_COUNT_START && index <= BIG_ATTACK_COUNT_END) return "BIG_ATTACK_COUNT_START+" + std::to_string(index - BIG_ATTACK_COUNT_START);
    if (index >= PIECE_ATTACKING_START && index <= PIECE_ATTACKING_END) return "PIECE_ATTACKING_START+" + std::to_string(index - PIECE_ATTACKING_START);
    if (index >= DISTINCT_PIECES_ATTACKING_START && index <= DISTINCT_PIECES_ATTACKING_END) return "DISTINCT_PIECES_ATTACKING_START+" + std::to_string(index - DISTINCT_PIECES_ATTACKING_START);
    if (index >= KING_ESCAPE_SQUARES_START && index <= KING_ESCAPE_SQUARES_END) return "KING_ESCAPE_SQUARES_START+" + std::to_string(index - KING_ESCAPE_SQUARES_START);
    if (index >= KING_TROPISM_START && index <= KING_TROPISM_END) return "KING_TROPISM_START+" + std::to_string(index - KING_TROPISM_START);
    if (index >= MOBILITY_START && index <= MOBILITY_END) return "MOBILITY_START+" + std::to_string(index - MOBILITY_START);
    if (index >= CONNECTED_PASSED_PAWNS_START && index <= CONNECTED_PASSED_PAWNS_END) return "CONNECTED_PASSED_PAWNS_START+" + std::to_string(index - CONNECTED_PASSED_PAWNS_START);
    if (index >= PIECE_SUPPORTED_PASSED_PAWNS_START && index <= PIECE_SUPPORTED_PASSED_PAWNS_END) return "PIECE_SUPPORTED_PASSED_PAWNS_START+" + std::to_string(index - PIECE_SUPPORTED_PASSED_PAWNS_START);
    if (index >= SAFE_ADVANCE_PASSED_PAWNS_START && index <= SAFE_ADVANCE_PASSED_PAWNS_END) return "SAFE_ADVANCE_PASSED_PAWNS_START+" + std::to_string(index - SAFE_ADVANCE_PASSED_PAWNS_START);
    if (index >= CLEAR_PATH_PASSED_PAWNS_START && index <= CLEAR_PATH_PASSED_PAWNS_END) return "CLEAR_PATH_PASSED_PAWNS_START+" + std::to_string(index - CLEAR_PATH_PASSED_PAWNS_START);

    return "PARAM_" + std::to_string(index);
}

static void print_parameter_non_zero_coverage(const parameters_t& parameters, const vector<Entry>& entries)
{
    vector<size_t> non_zero_counts(parameters.size(), 0);

    for (const auto& entry : entries)
    {
        for (const auto& coefficient : entry.coefficients)
        {
            if (coefficient.index < 0)
            {
                continue;
            }

            const auto parameter_index = static_cast<size_t>(coefficient.index);
            if (parameter_index < non_zero_counts.size())
            {
                non_zero_counts[parameter_index]++;
            }
        }
    }

    cout << "Parameter non-zero coverage:" << endl;
    cout << "Total positions: " << entries.size() << endl;

    size_t ever_non_zero = 0;
    for (size_t i = 0; i < non_zero_counts.size(); i++)
    {
        const auto count = non_zero_counts[i];
        if (count > 0)
        {
            ever_non_zero++;
        }

        const tune_t percentage = entries.empty()
            ? static_cast<tune_t>(0)
            : static_cast<tune_t>(100.0) * static_cast<tune_t>(count) / static_cast<tune_t>(entries.size());

        const auto idx = static_cast<int>(i);
        cout << "Param[" << i+PARAM_START << "] " << get_parameter_name(idx+PARAM_START) << ": "
            << count << " / " << entries.size()
            << " (" << percentage << "%)" << endl;
    }

    const tune_t used_percentage = parameters.empty()
        ? static_cast<tune_t>(0)
        : static_cast<tune_t>(100.0) * static_cast<tune_t>(ever_non_zero) / static_cast<tune_t>(parameters.size());

    cout << "Parameters ever non-zero: " << ever_non_zero << " / " << parameters.size()
        << " (" << used_percentage << "%)" << endl;
    cout << endl;
}

static uint64_t avalanche_split_key(uint64_t hash)
{
    hash ^= hash >> 30;
    hash *= 0xBF58476D1CE4E5B9ULL;
    hash ^= hash >> 27;
    hash *= 0x94D049BB133111EBULL;
    hash ^= hash >> 31;
    return hash;
}

static uint64_t stable_split_key(std::string_view text)
{
    uint64_t hash = 14695981039346656037ULL;
    for (const unsigned char ch : text)
    {
        hash ^= ch;
        hash *= 1099511628211ULL;
    }
    return avalanche_split_key(hash);
}

static void parse_fen(const bool side_to_move_wdl, const parameters_t& parameters, vector<Entry>& entries, const string& original_fen)
{
    if constexpr (print_data_entries)
    {
        cout << original_fen;
    }

    // Direkt FEN verwenden (keine Quiescence n�tig bei ruhigen Positionen)
    EvalResult eval_result = TuneEval::get_fen_eval_result(original_fen);

    /*const int tropism_index = KING_TROPISM_START - PARAM_START;
    if (tropism_index >= 0 &&
        tropism_index < static_cast<int>(eval_result.coefficients.size()) &&
        eval_result.coefficients[tropism_index] != 0)
    {
        cout << "KING_TROPISM_START != 0: " << original_fen << '\n';
    }*/

    Entry entry;
    entry.split_key = stable_split_key(original_fen);
    entry.white_to_move = get_fen_color_to_move(original_fen);
#if TAPERED
    entry.endgame_scale = eval_result.endgame_scale;
#endif
    entry.wdl = get_fen_wdl(original_fen, entry.white_to_move, entry.white_to_move, side_to_move_wdl);
    get_coefficient_entries(eval_result.coefficients, entry.coefficients, static_cast<int32_t>(parameters.size()));
#if TAPERED
    entry.phase = get_phase(original_fen);
#endif
    entry.additional_score = 0;
    if constexpr (TuneEval::includes_additional_score)
    {
        const tune_t score = linear_eval(entry, parameters);
        if constexpr (print_data_entries)
        {
            cout << " Eval: " << score << endl;
        }
        entry.additional_score = eval_result.additional_score;

    }

    entries.push_back(entry);
}

static void read_fens(const DataSource& source, const high_resolution_clock::time_point start, vector<string>& fens)
{
    cout << "Reading " << source.path;
    if (source.position_limit > 0)
    {
        cout << " (" << source.position_limit << " positions)";
    }
    cout << "..." << endl;

    ifstream file(source.path);
    if (!file)
    {
        cout << "Failed to open " << source.path << endl;
        throw runtime_error("Failed to open data source");
    }

    while (!file.eof())
    {
        if (source.position_limit > 0 && fens.size() >= source.position_limit)
        {
            break;
        }

        string original_fen;
        getline(file, original_fen);
        if (original_fen.empty())
        {
            break;
        }

        fens.push_back(original_fen);
    }

    print_elapsed(start);
    std::cout << "Read " << fens.size() << " positions from " << source.path << endl;
}

static void parse_fens(ThreadPool& thread_pool, const DataSource& source, const vector<string>& fens, const parameters_t& parameters, const high_resolution_clock::time_point time_start, vector<Entry>& entries)
{
    cout << "Parsing " << fens.size() << " positions..." << endl;
    array<vector<Entry>, data_load_thread_count> thread_entries;
    const auto side_to_move_wdl = source.side_to_move_wdl;
    constexpr int batch_size = 10000;
    mutex mut;
    queue<vector<string>> batches;
    vector<string> current_batch;
    for(const auto& fen : fens)
    {
        current_batch.push_back(fen);
        if (current_batch.size() == batch_size)
        {
            batches.emplace(current_batch);
            current_batch.clear();
        }
    }
    if(!current_batch.empty())
    {
        batches.emplace(current_batch);
    }

    for (int thread_id = 0; thread_id < data_load_thread_count; thread_id++)
    {
        thread_pool.enqueue([thread_id, &thread_entries, &mut, side_to_move_wdl, parameters, &batches, time_start]()
        {
            vector<Entry> entries;

            int position_count = 0;
            while(true)
            {
                vector<string> thread_batch;
                {
                    lock_guard lock(mut);
                    if(batches.empty())
                    {
                        break;
                    }
                    thread_batch = batches.front();
                    batches.pop();
                }

                constexpr auto thread_data_load_print_interval = TuneEval::data_load_print_interval / data_load_thread_count;
                for(auto& fen : thread_batch)
                {
                    parse_fen(side_to_move_wdl, parameters, entries, fen);
                    position_count++;
                    if (thread_id == 0 && position_count % thread_data_load_print_interval == 0)
                    {
                        print_elapsed(time_start);
                        std::cout << "Parsed ~" << position_count * data_load_thread_count << " positions..." << endl;
                    }
                }
            }

            thread_entries[thread_id] = entries;
        });
    }

    thread_pool.wait_for_completion();

    for (int thread_id = 0; thread_id < data_load_thread_count; thread_id++)
    {
        for(const Entry& entry : thread_entries[thread_id])
        {
            entries.push_back(entry);
        }
    }
}

static void load_fens(ThreadPool& thread_pool, const DataSource& source, const parameters_t& parameters, const high_resolution_clock::time_point start, vector<Entry>& entries)
{
    vector<string> fens;
    read_fens(source, start, fens);
    parse_fens(thread_pool, source, fens, parameters, start, entries);
}

static tune_t sigmoid_from_logit(const tune_t logit)
{
    if (logit >= 0)
    {
        return static_cast<tune_t>(1)
            / (static_cast<tune_t>(1) + exp(-logit));
    }
    const tune_t exp_logit = exp(logit);
    return exp_logit / (static_cast<tune_t>(1) + exp_logit);
}

static tune_t cross_entropy_from_logit(const tune_t label, const tune_t logit)
{
    return max(logit, static_cast<tune_t>(0))
        - label * logit
        + log1p(exp(-abs(logit)));
}

static parameters_t zero_parameters(const size_t size)
{
#if TAPERED
    return parameters_t(size, pair_t{});
#else
    return parameters_t(size, 0);
#endif
}

static tune_t parameter_dot(const parameters_t& lhs, const parameters_t& rhs)
{
    tune_t result = 0;
    for (size_t index = 0; index < lhs.size(); ++index)
    {
#if TAPERED
        for (int phase = 0; phase < 2; ++phase)
        {
            result += lhs[index][phase] * rhs[index][phase];
        }
#else
        result += lhs[index] * rhs[index];
#endif
    }
    return result;
}

static tune_t parameter_infinity_norm(const parameters_t& parameters)
{
    tune_t result = 0;
    for (size_t index = 0; index < parameters.size(); ++index)
    {
#if TAPERED
        for (int phase = 0; phase < 2; ++phase)
        {
            result = max(result, abs(parameters[index][phase]));
        }
#else
        result = max(result, abs(parameters[index]));
#endif
    }
    return result;
}

static void add_scaled(
    parameters_t& destination,
    const parameters_t& source,
    const tune_t scale)
{
    for (size_t index = 0; index < destination.size(); ++index)
    {
#if TAPERED
        for (int phase = 0; phase < 2; ++phase)
        {
            destination[index][phase] += scale * source[index][phase];
        }
#else
        destination[index] += scale * source[index];
#endif
    }
}

static parameters_t parameter_difference(
    const parameters_t& lhs,
    const parameters_t& rhs)
{
    parameters_t result = lhs;
    add_scaled(result, rhs, static_cast<tune_t>(-1));
    return result;
}

static void scale_parameters(parameters_t& parameters, const tune_t scale)
{
    for (size_t index = 0; index < parameters.size(); ++index)
    {
#if TAPERED
        for (int phase = 0; phase < 2; ++phase)
        {
            parameters[index][phase] *= scale;
        }
#else
        parameters[index] *= scale;
#endif
    }
}

struct ObjectiveResult
{
    tune_t loss = 0;
    parameters_t gradient;
    parameters_t hessian_diagonal;
};

static ObjectiveResult compute_objective(
    ThreadPool& thread_pool,
    const vector<Entry>& entries,
    const parameters_t& parameters,
    const tune_t K,
    const parameters_t& anchor,
    const tune_t l2_regularization,
    const bool need_derivatives)
{
    if (entries.empty())
    {
        throw runtime_error("Cannot evaluate an empty dataset");
    }

    struct ThreadObjective
    {
        tune_t loss = 0;
        parameters_t gradient;
        parameters_t hessian_diagonal;
    };

    array<ThreadObjective, thread_count> thread_objectives;
    for (int thread_id = 0; thread_id < thread_count; ++thread_id)
    {
        thread_pool.enqueue([
            thread_id,
            &thread_objectives,
            &entries,
            &parameters,
            K,
            need_derivatives]()
        {
            ThreadObjective result;
            if (need_derivatives)
            {
                result.gradient = zero_parameters(parameters.size());
                result.hessian_diagonal = zero_parameters(parameters.size());
            }

            const size_t start =
                entries.size() * static_cast<size_t>(thread_id) / thread_count;
            const size_t end =
                entries.size() * static_cast<size_t>(thread_id + 1) / thread_count;
            const tune_t logit_scale = K / static_cast<tune_t>(400);

            for (size_t index = start; index < end; ++index)
            {
                const Entry& entry = entries[index];
                const tune_t eval = linear_eval(entry, parameters);
                const tune_t logit = logit_scale * eval;
                const tune_t probability = sigmoid_from_logit(logit);
                result.loss += cross_entropy_from_logit(entry.wdl, logit);

                if (!need_derivatives)
                {
                    continue;
                }

                const tune_t residual = probability - entry.wdl;
                const tune_t curvature = probability
                    * (static_cast<tune_t>(1) - probability);
#if TAPERED
                const tune_t mg_phase =
                    entry.phase / static_cast<tune_t>(24);
                const tune_t eg_phase =
                    (static_cast<tune_t>(24) - entry.phase)
                    / static_cast<tune_t>(24);
#endif

                for (const CoefficientEntry& coefficient : entry.coefficients)
                {
#if TAPERED
                    const tune_t mg_logit_derivative =
                        logit_scale * coefficient.value * mg_phase;
                    const tune_t eg_logit_derivative =
                        logit_scale * coefficient.value * eg_phase
                        * entry.endgame_scale;
                    result.gradient[coefficient.index]
                        [static_cast<int32_t>(PhaseStages::Midgame)]
                        += residual * mg_logit_derivative;
                    result.gradient[coefficient.index]
                        [static_cast<int32_t>(PhaseStages::Endgame)]
                        += residual * eg_logit_derivative;
                    result.hessian_diagonal[coefficient.index]
                        [static_cast<int32_t>(PhaseStages::Midgame)]
                        += curvature * mg_logit_derivative * mg_logit_derivative;
                    result.hessian_diagonal[coefficient.index]
                        [static_cast<int32_t>(PhaseStages::Endgame)]
                        += curvature * eg_logit_derivative * eg_logit_derivative;
#else
                    const tune_t logit_derivative =
                        logit_scale * coefficient.value;
                    result.gradient[coefficient.index]
                        += residual * logit_derivative;
                    result.hessian_diagonal[coefficient.index]
                        += curvature * logit_derivative * logit_derivative;
#endif
                }
            }
            thread_objectives[thread_id] = std::move(result);
        });
    }

    thread_pool.wait_for_completion();

    ObjectiveResult result;
    if (need_derivatives)
    {
        result.gradient = zero_parameters(parameters.size());
        result.hessian_diagonal = zero_parameters(parameters.size());
    }

    for (const ThreadObjective& thread_result : thread_objectives)
    {
        result.loss += thread_result.loss;
        if (need_derivatives)
        {
            add_scaled(result.gradient, thread_result.gradient, 1);
            add_scaled(
                result.hessian_diagonal,
                thread_result.hessian_diagonal,
                1);
        }
    }

    const tune_t inverse_count =
        static_cast<tune_t>(1) / static_cast<tune_t>(entries.size());
    result.loss *= inverse_count;
    if (need_derivatives)
    {
        scale_parameters(result.gradient, inverse_count);
        scale_parameters(result.hessian_diagonal, inverse_count);
    }

    for (size_t index = 0; index < parameters.size(); ++index)
    {
#if TAPERED
        for (int phase = 0; phase < 2; ++phase)
        {
            const tune_t delta =
                parameters[index][phase] - anchor[index][phase];
            result.loss += static_cast<tune_t>(0.5)
                * l2_regularization * delta * delta;
            if (need_derivatives)
            {
                result.gradient[index][phase] += l2_regularization * delta;
                result.hessian_diagonal[index][phase] += l2_regularization;
            }
        }
#else
        const tune_t delta = parameters[index] - anchor[index];
        result.loss += static_cast<tune_t>(0.5)
            * l2_regularization * delta * delta;
        if (need_derivatives)
        {
            result.gradient[index] += l2_regularization * delta;
            result.hessian_diagonal[index] += l2_regularization;
        }
#endif
    }
    return result;
}

struct KObjective
{
    tune_t loss = 0;
    tune_t gradient = 0;
    tune_t curvature = 0;
};

static KObjective compute_k_objective(
    ThreadPool& thread_pool,
    const vector<Entry>& entries,
    const parameters_t& parameters,
    const tune_t K)
{
    array<KObjective, thread_count> thread_results{};
    for (int thread_id = 0; thread_id < thread_count; ++thread_id)
    {
        thread_pool.enqueue([
            thread_id,
            &thread_results,
            &entries,
            &parameters,
            K]()
        {
            KObjective result;
            const size_t start =
                entries.size() * static_cast<size_t>(thread_id) / thread_count;
            const size_t end =
                entries.size() * static_cast<size_t>(thread_id + 1) / thread_count;

            for (size_t index = start; index < end; ++index)
            {
                const Entry& entry = entries[index];
                const tune_t eval_scale =
                    linear_eval(entry, parameters) / static_cast<tune_t>(400);
                const tune_t logit = K * eval_scale;
                const tune_t probability = sigmoid_from_logit(logit);
                result.loss += cross_entropy_from_logit(entry.wdl, logit);
                result.gradient +=
                    (probability - entry.wdl) * eval_scale;
                result.curvature += probability
                    * (static_cast<tune_t>(1) - probability)
                    * eval_scale * eval_scale;
            }
            thread_results[thread_id] = result;
        });
    }
    thread_pool.wait_for_completion();

    KObjective result;
    for (const KObjective& thread_result : thread_results)
    {
        result.loss += thread_result.loss;
        result.gradient += thread_result.gradient;
        result.curvature += thread_result.curvature;
    }
    const tune_t inverse_count =
        static_cast<tune_t>(1) / static_cast<tune_t>(entries.size());
    result.loss *= inverse_count;
    result.gradient *= inverse_count;
    result.curvature *= inverse_count;
    return result;
}

static tune_t find_optimal_k(
    ThreadPool& thread_pool,
    const vector<Entry>& entries,
    const parameters_t& parameters)
{
    constexpr tune_t minimum_k = 1e-6;
    constexpr tune_t armijo = 1e-4;
    tune_t K = 2.5;
    KObjective objective =
        compute_k_objective(thread_pool, entries, parameters, K);

    for (int iteration = 0; iteration < 50; ++iteration)
    {
        if (abs(objective.gradient) <= 1e-10)
        {
            break;
        }
        if (K <= minimum_k && objective.gradient >= 0)
        {
            break;
        }
        if (!(objective.curvature > 0) || !isfinite(objective.curvature))
        {
            throw runtime_error("K optimization has no positive curvature");
        }

        const tune_t direction =
            -objective.gradient / objective.curvature;
        tune_t step = 1;
        bool accepted = false;

        for (int line_search = 0; line_search < 30; ++line_search)
        {
            const tune_t candidate_k =
                max(minimum_k, K + step * direction);
            const KObjective candidate =
                compute_k_objective(
                    thread_pool,
                    entries,
                    parameters,
                    candidate_k);
            if (candidate.loss <= objective.loss
                + armijo * objective.gradient * (candidate_k - K))
            {
                K = candidate_k;
                objective = candidate;
                accepted = true;
                break;
            }
            step *= static_cast<tune_t>(0.5);
        }

        if (!accepted)
        {
            throw runtime_error("K line search failed");
        }
    }

    cout << "K cross-entropy = " << objective.loss << endl;
    return K;
}

static vector<Entry> split_validation_entries(vector<Entry>& training_entries)
{
    const tune_t validation_fraction = TuneEval::validation_fraction;
    if (validation_fraction <= 0)
    {
        return {};
    }
    if (validation_fraction >= 1)
    {
        throw runtime_error("validation_fraction must be less than 1");
    }

    const long double hash_range =
        static_cast<long double>(numeric_limits<uint64_t>::max());
    const uint64_t validation_threshold = static_cast<uint64_t>(
        validation_fraction * hash_range);

    vector<Entry> training;
    vector<Entry> validation;
    training.reserve(training_entries.size());
    validation.reserve(static_cast<size_t>(
        validation_fraction * training_entries.size()) + 1);

    for (Entry& entry : training_entries)
    {
        const uint64_t seeded_key = avalanche_split_key(
            entry.split_key
            ^ (static_cast<uint64_t>(TuneEval::validation_seed)
                * 0x9E3779B97F4A7C15ULL));
        if (seeded_key
            <= validation_threshold)
        {
            validation.push_back(std::move(entry));
        }
        else
        {
            training.push_back(std::move(entry));
        }
    }

    if (training.empty() || validation.empty())
    {
        throw runtime_error(
            "Training/validation split produced an empty partition");
    }
    training_entries = std::move(training);
    return validation;
}

struct LbfgsHistory
{
    vector<parameters_t> parameter_steps;
    vector<parameters_t> gradient_steps;
    vector<tune_t> inverse_curvatures;
};

static parameters_t get_lbfgs_direction(
    const ObjectiveResult& objective,
    const LbfgsHistory& history)
{
    parameters_t direction = objective.gradient;
    vector<tune_t> alpha(history.parameter_steps.size(), 0);

    for (size_t reverse_index = history.parameter_steps.size();
        reverse_index > 0;
        --reverse_index)
    {
        const size_t index = reverse_index - 1;
        alpha[index] = history.inverse_curvatures[index]
            * parameter_dot(history.parameter_steps[index], direction);
        add_scaled(
            direction,
            history.gradient_steps[index],
            -alpha[index]);
    }

    if (history.parameter_steps.empty())
    {
        for (size_t index = 0; index < direction.size(); ++index)
        {
#if TAPERED
            for (int phase = 0; phase < 2; ++phase)
            {
                direction[index][phase] /=
                    max(objective.hessian_diagonal[index][phase], 1e-12);
            }
#else
            direction[index] /=
                max(objective.hessian_diagonal[index], 1e-12);
#endif
        }
    }
    else
    {
        const parameters_t& last_parameter_step =
            history.parameter_steps.back();
        const parameters_t& last_gradient_step =
            history.gradient_steps.back();
        const tune_t curvature =
            parameter_dot(last_parameter_step, last_gradient_step);
        const tune_t gradient_curvature =
            parameter_dot(last_gradient_step, last_gradient_step);
        const tune_t initial_hessian_scale =
            gradient_curvature > 0
            ? curvature / gradient_curvature
            : static_cast<tune_t>(1);
        scale_parameters(direction, initial_hessian_scale);
    }

    for (size_t index = 0; index < history.parameter_steps.size(); ++index)
    {
        const tune_t beta = history.inverse_curvatures[index]
            * parameter_dot(history.gradient_steps[index], direction);
        add_scaled(
            direction,
            history.parameter_steps[index],
            alpha[index] - beta);
    }

    scale_parameters(direction, static_cast<tune_t>(-1));
    return direction;
}

static parameters_t optimize_parameters(
    ThreadPool& thread_pool,
    const vector<Entry>& training_entries,
    const vector<Entry>& validation_entries,
    parameters_t parameters,
    const parameters_t& anchor,
    const tune_t K,
    const high_resolution_clock::time_point start)
{
    constexpr tune_t armijo = 1e-4;
    constexpr tune_t minimum_step = 1e-12;
    LbfgsHistory history;
    ObjectiveResult objective = compute_objective(
        thread_pool,
        training_entries,
        parameters,
        K,
        anchor,
        TuneEval::l2_regularization,
        true);

    tune_t best_validation_loss = validation_entries.empty()
        ? objective.loss
        : compute_objective(
            thread_pool,
            validation_entries,
            parameters,
            K,
            anchor,
            0,
            false).loss;
    parameters_t best_parameters = parameters;
    int32_t checks_without_improvement = 0;

    cout << "Initial training objective = " << objective.loss << endl;
    if (!validation_entries.empty())
    {
        cout << "Initial validation cross-entropy = "
             << best_validation_loss << endl;
    }

    for (int32_t iteration = 1;
        iteration <= TuneEval::max_epoch;
        ++iteration)
    {
        const tune_t gradient_norm =
            parameter_infinity_norm(objective.gradient);
        if (gradient_norm <= TuneEval::gradient_tolerance)
        {
            cout << "Converged: gradient infinity norm "
                 << gradient_norm << endl;
            break;
        }

        parameters_t direction =
            get_lbfgs_direction(objective, history);
        tune_t directional_derivative =
            parameter_dot(objective.gradient, direction);
        if (!(directional_derivative < 0)
            || !isfinite(directional_derivative))
        {
            history = {};
            direction = objective.gradient;
            scale_parameters(direction, static_cast<tune_t>(-1));
            directional_derivative =
                -parameter_dot(objective.gradient, objective.gradient);
        }

        tune_t step = 1;
        parameters_t candidate_parameters;
        ObjectiveResult candidate_loss;
        bool accepted = false;
        for (int line_search = 0; line_search < 40; ++line_search)
        {
            candidate_parameters = parameters;
            add_scaled(candidate_parameters, direction, step);
            candidate_loss = compute_objective(
                thread_pool,
                training_entries,
                candidate_parameters,
                K,
                anchor,
                TuneEval::l2_regularization,
                false);

            if (isfinite(candidate_loss.loss)
                && candidate_loss.loss <= objective.loss
                    + armijo * step * directional_derivative)
            {
                accepted = true;
                break;
            }
            step *= static_cast<tune_t>(0.5);
            if (step < minimum_step)
            {
                break;
            }
        }

        if (!accepted)
        {
            cout << "Stopping: line search could not find a lower objective"
                 << endl;
            break;
        }

        ObjectiveResult candidate_objective = compute_objective(
            thread_pool,
            training_entries,
            candidate_parameters,
            K,
            anchor,
            TuneEval::l2_regularization,
            true);
        parameters_t parameter_step =
            parameter_difference(candidate_parameters, parameters);
        parameters_t gradient_step =
            parameter_difference(
                candidate_objective.gradient,
                objective.gradient);
        const tune_t curvature =
            parameter_dot(parameter_step, gradient_step);
        const tune_t curvature_scale = sqrt(
            max(parameter_dot(parameter_step, parameter_step), 0.0)
            * max(parameter_dot(gradient_step, gradient_step), 0.0));
        if (curvature > 1e-12 * max(curvature_scale, 1.0))
        {
            if (history.parameter_steps.size()
                == static_cast<size_t>(TuneEval::lbfgs_history_size))
            {
                history.parameter_steps.erase(
                    history.parameter_steps.begin());
                history.gradient_steps.erase(
                    history.gradient_steps.begin());
                history.inverse_curvatures.erase(
                    history.inverse_curvatures.begin());
            }
            history.parameter_steps.push_back(std::move(parameter_step));
            history.gradient_steps.push_back(std::move(gradient_step));
            history.inverse_curvatures.push_back(
                static_cast<tune_t>(1) / curvature);
        }

        const tune_t previous_loss = objective.loss;
        parameters = std::move(candidate_parameters);
        objective = std::move(candidate_objective);

        const bool validation_check =
            iteration % TuneEval::validation_interval == 0
            || iteration == TuneEval::max_epoch;
        if (validation_check)
        {
            const tune_t validation_loss = validation_entries.empty()
                ? objective.loss
                : compute_objective(
                    thread_pool,
                    validation_entries,
                    parameters,
                    K,
                    anchor,
                    0,
                    false).loss;
            print_elapsed(start);
            cout << "Iteration " << iteration
                 << ", train objective " << setprecision(17)
                 << objective.loss
                 << ", validation CE " << validation_loss
                 << ", |gradient|_inf "
                 << parameter_infinity_norm(objective.gradient)
                 << ", step " << step << endl;

            if (validation_loss
                < best_validation_loss - TuneEval::validation_min_delta)
            {
                best_validation_loss = validation_loss;
                best_parameters = parameters;
                checks_without_improvement = 0;
            }
            else
            {
                ++checks_without_improvement;
                if (!validation_entries.empty()
                    && checks_without_improvement
                        >= TuneEval::early_stopping_patience)
                {
                    cout << "Early stopping: validation loss did not improve "
                         << "for " << checks_without_improvement
                         << " checks" << endl;
                    break;
                }
            }
        }

        const tune_t relative_improvement =
            (previous_loss - objective.loss)
            / max(abs(previous_loss), static_cast<tune_t>(1));
        if (relative_improvement >= 0
            && relative_improvement <= TuneEval::relative_loss_tolerance)
        {
            cout << "Converged: relative training-objective improvement "
                 << relative_improvement << endl;
            break;
        }
    }

    const tune_t final_validation_loss = validation_entries.empty()
        ? objective.loss
        : compute_objective(
            thread_pool,
            validation_entries,
            parameters,
            K,
            anchor,
            0,
            false).loss;
    if (final_validation_loss
        < best_validation_loss - TuneEval::validation_min_delta)
    {
        best_validation_loss = final_validation_loss;
        best_parameters = parameters;
    }

    cout << "Best validation cross-entropy = "
         << best_validation_loss << endl;
    return best_parameters;
}


void Tuner::run(const std::vector<DataSource>& sources)
{
    cout << "Starting tuning" << endl << endl;
    const auto start = high_resolution_clock::now();

    cout << "Starting thread pool..." << endl;
    ThreadPool thread_pool;
    thread_pool.start(thread_count);

    cout << "Getting initial parameters..." << endl;
    auto parameters = TuneEval::get_initial_parameters();
    cout << "Got " << parameters.size() << " parameters" << endl;

    cout << "Initial parameters:" << endl;
    TuneEval::print_parameters(parameters);

    vector<Entry> entries;

    for (const auto& source : sources)
    {
        load_fens(thread_pool, source, parameters, start, entries);
    }
    cout << "Data loading complete" << endl << endl;

    vector<Entry> validation_entries =
        split_validation_entries(entries);
    cout << "Training positions: " << entries.size() << endl;
    cout << "Validation positions: " << validation_entries.size() << endl;
    cout << "Training dataset:" << endl;
    print_statistics(parameters, entries);
    if (!validation_entries.empty())
    {
        cout << "Validation dataset:" << endl;
        print_statistics(parameters, validation_entries);
    }
    cout << "Training feature coverage:" << endl;
    print_parameter_non_zero_coverage(parameters, entries);

    if constexpr (TuneEval::retune_from_zero)
    {
        for (auto& parameter : parameters)
        {
#if TAPERED
            parameter[static_cast<int>(PhaseStages::Midgame)] = static_cast<tune_t>(0);
            parameter[static_cast<int>(PhaseStages::Endgame)] = static_cast<tune_t>(0);
#else
            parameter = static_cast<tune_t>(0);
#endif            
        }
    }

    cout << "Initial parameters:" << endl;
    TuneEval::print_parameters(parameters);
    const parameters_t anchor = parameters;

    tune_t K;
    if constexpr (TuneEval::preferred_k <= 0)
    {
        cout << "Finding optimal K..." << endl;
        K = find_optimal_k(thread_pool, entries, parameters);
    }
    else
    {
        cout << "Using predefined K = " << TuneEval::preferred_k << endl;
        K = TuneEval::preferred_k;
    }
    cout << "K = " << K << endl;

    parameters = optimize_parameters(
        thread_pool,
        entries,
        validation_entries,
        parameters,
        anchor,
        K,
        start);

    thread_pool.stop();

    cout << "Best validation parameters:" << endl;
    TuneEval::print_parameters(parameters);
}
