#include "TuneEval.h"
#include <cmath>
#include <iostream>
#include <string>
#include "pst.h"
#include "utils.h"
#include "evaluation.h"
#include "bitboard_masks.h"
#include "see.h"

//static constexpr int flip_square(int sq) {
//    return sq ^ 56;
//}
void TuneEval::normalize_pst(parameters_t& params) {
    // Nur normalisieren, wenn der komplette PST-Bereich (Pawn..King) im Tuning-Fenster liegt.
    // PARAM_END wird im Projekt exklusiv verwendet (< PARAM_END), daher > KING_PST_END.
    if (!(PARAM_START <= PAWN_PST_START && PARAM_END > KING_PST_END)) {
        return;
    }

    struct PstNormEntry {
        int pst_start;
        int value_param;
    };

    const PstNormEntry pst_norm_entries[] = {
        { PAWN_PST_START,   PAWN   },
        { KNIGHT_PST_START, KNIGHT },
        { BISHOP_PST_START, BISHOP },
        { ROOK_PST_START,   ROOK   },
        { QUEEN_PST_START,  QUEEN  },
    };

    auto is_tuned = [](int global_index) {
        return global_index >= PARAM_START && global_index < PARAM_END;
    };

    auto local_index_of = [](int global_index) {
        return global_index - PARAM_START;
    };

    auto add_phase_value = [&](int global_index, int phase, double delta) {
        params[local_index_of(global_index)][phase] += delta;
    };

    auto get_phase_value = [&](int global_index, int phase) -> double {
        return params[local_index_of(global_index)][phase];
    };

    for (const auto& entry : pst_norm_entries) {
        int start_sq = 0;
        int end_sq = 64;
        if (entry.pst_start == PAWN_PST_START) {
            start_sq = 8;
            end_sq = 56;
        }

        // Sicherheitscheck: Ohne value_param im Tuning-Fenster keine Normalisierung
        // (sonst könnte die Summe nicht auf piece value verschoben werden).
        if (!is_tuned(entry.value_param)) {
            continue;
        }

        std::vector<int> tuned_pst_indices;
        tuned_pst_indices.reserve(end_sq - start_sq);

        for (int sq = start_sq; sq < end_sq; ++sq) {
            const int global_index = entry.pst_start + sq;
            if (is_tuned(global_index)) {
                tuned_pst_indices.push_back(global_index);
            }
        }

        if (tuned_pst_indices.empty()) {
            continue;
        }

        for (int phase = 0; phase < 2; ++phase) {
            double sum = 0.0;
            for (const int idx : tuned_pst_indices) {
                sum += get_phase_value(idx, phase);
            }

            const double avg = sum / static_cast<double>(tuned_pst_indices.size());

            for (const int idx : tuned_pst_indices) {
                add_phase_value(idx, phase, -avg);
            }

            add_phase_value(entry.value_param, phase, avg);
        }
    }
}
parameters_t TuneEval::get_initial_parameters() {
    parameters_t params(PARAM_LENGTH);
    for(int i=PARAM_START;i<PARAM_END;i++) {
        params[i - PARAM_START] = { static_cast<double>(EvalWeights[i].mg_score), static_cast<double>(EvalWeights[i].eg_score) };
	}
    return params;
}


EvalResult TuneEval::get_external_eval_result(const Board& board) {
    EvalResult result;
    Trace trace;
    result.additional_score = evaluate<true>(board, &trace);

    result.coefficients.resize(PARAM_LENGTH);
    for(int i=PARAM_START;i<PARAM_END;i++){
        result.coefficients[i-PARAM_START] = trace.counts[i];
	}

    return result;
}

EvalResult TuneEval::get_fen_eval_result(const std::string& fen) {
    Board board(fen);
    return get_external_eval_result(board);
}
namespace {
    std::string get_parameter_name(int index)
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
        case PAWN_SHIELD_BONUS: return "PAWN_SHIELD_BONUS";
        case DIRECTLY_ON_OPEN_FILE_NEXT_TO_OPEN_PENALTY: return "DIRECTLY_ON_OPEN_FILE_NEXT_TO_OPEN_PENALTY";
        case DIRECTLY_ON_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY: return "DIRECTLY_ON_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY";
        case NEXT_TO_OPEN_FILE_PENALTY: return "NEXT_TO_OPEN_FILE_PENALTY";
        case DIRECTLY_ON_SEMI_OPEN_FILE_NEXT_TO_OPEN_PENALTY: return "DIRECTLY_ON_SEMI_OPEN_FILE_NEXT_TO_OPEN_PENALTY";
        case DIRECTLY_ON_SEMI_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY: return "DIRECTLY_ON_SEMI_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY";
        case NEXT_TO_SEMI_OPEN_FILE_PENALTY: return "NEXT_TO_SEMI_OPEN_FILE_PENALTY";
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
        if (index >= ISOLANI_START && index <= ISOLANI_END) return "ISOLANI_START+" + std::to_string(index - ISOLANI_START);
        if (index >= BLOCKED_ISOLANI_START && index <= BLOCKED_ISOLANI_END) return "BLOCKED_ISOLANI_START+" + std::to_string(index - BLOCKED_ISOLANI_START);
        if (index >= DOUBLE_PAWN_FILE_START && index <= DOUBLE_PAWN_FILE_END) return "DOUBLE_PAWN_FILE_START+" + std::to_string(index - DOUBLE_PAWN_FILE_START);
        if (index >= NEXT_TO_OPEN_DIAGONAL_PENALTY_START && index <= NEXT_TO_OPEN_DIAGONAL_PENALTY_END) return "NEXT_TO_OPEN_DIAGONAL_PENALTY_START+" + std::to_string(index - NEXT_TO_OPEN_DIAGONAL_PENALTY_START);
        if (index >= MOBILITY_START && index <= MOBILITY_END) return "MOBILITY_START+" + std::to_string(index - MOBILITY_START);
        if (index >= ROOK_BEHIND_FREE_PAWN_START && index <= ROOK_BEHIND_FREE_PAWN_END) return "ROOK_BEHIND_FREE_PAWN_START+" + std::to_string(index - ROOK_BEHIND_FREE_PAWN_START);

        return "PARAM_" + std::to_string(index);
    }

    double get_phase_value(const parameters_t& parameters, int global_index, int phase)
    {
        const int local_index = global_index - PARAM_START;
        if (local_index >= 0 && local_index < static_cast<int>(parameters.size()))
        {
            return parameters[local_index][phase];
        }

        return (phase == static_cast<int>(PhaseStages::Midgame))
            ? static_cast<double>(EvalWeights[global_index].mg_score)
            : static_cast<double>(EvalWeights[global_index].eg_score);
    }
}

#include <iostream>
#include <iomanip> // Required for std::setw
#include <cmath>

void TuneEval::print_parameters(parameters_t& parameters) {
    const int mg = static_cast<int>(PhaseStages::Midgame);
    const int eg = static_cast<int>(PhaseStages::Endgame);
    //normalize_pst(parameters);
    const double pawn_mg = get_phase_value(parameters, PAWN, mg);

    const double mg_scale = (pawn_mg != 0.0) ? 100.0 / pawn_mg : 0.0;
    const double eg_scale = (pawn_mg != 0.0) ? 100.0 / pawn_mg : 0.0;

    std::cout << "EvaluationResult EvalWeights[PARAM_COUNT] = {\n";

    for (int idx = PAWN; idx < PARAM_END; ++idx)
    {
        // Check if the current index is the start of an 8x8 piece-square table
        if (idx == PAWN_PST_START || idx == KNIGHT_PST_START ||
            idx == BISHOP_PST_START || idx == ROOK_PST_START ||
            idx == QUEEN_PST_START || idx == KING_PST_START ||
            idx == PASSED_PAWNS_START || idx == ISOLANI_START ||
            idx == BLOCKED_ISOLANI_START)
        {
            std::cout << "\t// " << get_parameter_name(idx) << "\n";

            // Loop through the 64 squares (8 ranks x 8 files)
            for (int rank = 0; rank < 8; ++rank) {
                std::cout << "\t";
                for (int file = 0; file < 8; ++file) {
                    int sq_idx = idx + rank * 8 + file;
                    int mg_value = static_cast<int>(std::round(get_phase_value(parameters, sq_idx, mg) * mg_scale));
                    int eg_value = static_cast<int>(std::round(get_phase_value(parameters, sq_idx, eg) * eg_scale));

                    // Format the output to align properly in columns
                    std::cout << "{" << std::setw(3) << mg_value << ", " << std::setw(3) << eg_value << "}, ";
                }
                std::cout << "\n"; // Newline after every rank
            }

            // Advance the index by 63. 
            // The for-loop will do ++idx, advancing it to a total of 64 (the next parameter block)
            idx += 63;
        }
        else
        {
            // Standard formatting for single parameters
            const auto mg_value = static_cast<int>(std::round(get_phase_value(parameters, idx, mg) * mg_scale));
            const auto eg_value = static_cast<int>(std::round(get_phase_value(parameters, idx, eg) * eg_scale));
            std::cout << "\t{" << mg_value << ", " << eg_value << "},\t// " << get_parameter_name(idx) << '\n';
        }
    }
    std::cout << "};\n";
}