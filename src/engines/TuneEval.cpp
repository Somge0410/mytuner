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
    struct PstNormEntry {
        int pst_start;
        int value_param;
    };
    const PstNormEntry pst_norm_entries[] = {
        { PAWN_PST_START,   PAWN   },
        { KNIGHT_PST_START, KNIGHT  },
        { BISHOP_PST_START, BISHOP  },
        { ROOK_PST_START,   ROOK    },
        { QUEEN_PST_START,  QUEEN   },
    };

    for (const auto& entry : pst_norm_entries) {
        for (int phase = 0; phase < 2; phase++) {
            double sum = 0.0;
            int start = 0;
            int end = 64;
            if (entry.pst_start == PAWN_PST_START) {
                start = 8;
                end = 56;
            }
            for (int sq = start; sq < end; sq++) {
                sum += params[entry.pst_start + sq][phase];
            }
            double avg = sum / (end - start);
            for (int sq = start; sq < end; sq++) {
                params[entry.pst_start + sq][phase] -= avg;
            }
            params[entry.value_param][phase] += avg;
        }
    }
}
parameters_t TuneEval::get_initial_parameters() {
    parameters_t params(PARAM_COUNT);
    for(int i=0;i<PARAM_COUNT;i++) {
        params[i] = { static_cast<double>(EvalWeights[i].mg_score), static_cast<double>(EvalWeights[i].eg_score) };
	}
    return params;
}


EvalResult TuneEval::get_external_eval_result(const Board& board) {
    EvalResult result;
    result.additional_score = 0;

    Trace trace;
    evaluate<true>(board, &trace);

    result.coefficients.resize(PARAM_COUNT);
    for(int i=0;i<=PARAM_COUNT;i++){
        result.coefficients[i] = trace.counts[i];
	}

    return result;
}

EvalResult TuneEval::get_fen_eval_result(const std::string& fen) {
    Board board(fen);
    return get_external_eval_result(board);
}

void TuneEval::print_parameters(const parameters_t& parameters) {
    const int mg = static_cast<int>(PhaseStages::Midgame);
    const int eg = static_cast<int>(PhaseStages::Endgame);

    const double mg_scale = 100.0 / parameters[PAWN][mg];
    const double eg_scale = 100.0 / parameters[PAWN][eg];

    auto mg_int = [&](int idx) { return static_cast<int>(std::round(parameters[idx][mg] * mg_scale)); };
    auto eg_int = [&](int idx) { return static_cast<int>(std::round(parameters[idx][eg] * eg_scale)); };

    auto print_entry = [&](int idx, const std::string& comment) {
        std::cout << "\t{" << mg_int(idx) << "," << eg_int(idx) << "},\t  // " << comment << '\n';
        };

    auto print_range = [&](int start, int end, const char* section_comment, const char* base_name) {
        std::cout << "\t// " << section_comment << '\n';
        for (int i = start; i <= end; ++i) {
            if (i == start) {
                print_entry(i, base_name);
            }
            else {
                print_entry(i, std::string(base_name) + "+" + std::to_string(i - start));
            }
        }
        };

    std::cout << "EvaluationResult EvalWeights[PARAM_COUNT] = {\n";

    print_entry(PAWN, "PAWN");
    print_entry(KNIGHT, "KNIGHT");
    print_entry(BISHOP, "BISHOP");
    print_entry(ROOK, "ROOK");
    print_entry(QUEEN, "QUEEN");

    std::cout << "\t// Positional weights\n";
    print_range(PAWN_PST_START, PAWN_PST_END, "PAWN Values", "PAWN_PST_START");
    print_range(KNIGHT_PST_START, KNIGHT_PST_END, "Knight PST values", "KNIGHT_PST_START");
    print_range(BISHOP_PST_START, BISHOP_PST_END, "BISHOP PST values", "BISHOP_PST_START");
    print_range(ROOK_PST_START, ROOK_PST_END, "ROOK PST values", "ROOK_PST_START");
    print_range(QUEEN_PST_START, QUEEN_PST_END, "Queen PST values", "QUEEN_PST_START");
    print_range(KING_PST_START, KING_PST_END, "Knight PST values", "KING_PST_START");

    std::cout << "\t//PAWN EVAL\n";
    print_range(PASSED_PAWNS_START, PASSED_PAWNS_END, "PASSED PST values", "PASSED_PAWNS_START");
    print_range(ISOLANI_START, ISOLANI_END, "Knight PST values", "ISOLANI_START");
    print_range(BLOCKED_ISOLANI_START, BLOCKED_ISOLANI_END, "Knight PST values", "BLOCKED_ISOLANI_START");

    print_entry(FORWARD_BLOCKED_BACKWARD, "FORWARD_BLOCKED_BACKWARD");
    print_entry(FORWARD_CONTROLLED_BACKWARD, "FORWARD_CONTROLLED_BACKWARD");
    print_entry(FREE_TO_ADV_BACKWARD, "FREE_TO_ADV_BACKWARD");
    print_range(DOUBLE_PAWN_FILE_START, DOUBLE_PAWN_FILE_END, "DOUBLE_PAWN_FILE values", "DOUBLE_PAWN_FILE_START");
    print_entry(PAWN_SHIELD_BONUS, "PAWN_SHIELD_BONUS,");
    print_entry(DIRECTLY_ON_OPEN_FILE_NEXT_TO_OPEN_PENALTY, "DIRECTLY_ON_OPEN_FILE_NEXT_TO_OPEN_PENALTY,");
    print_entry(DIRECTLY_ON_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY, "DIRECTLY_ON_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY,");
    print_entry(NEXT_TO_OPEN_FILE_PENALTY, "NEXT_TO_OPEN_FILE_PENALTY,");
    print_entry(DIRECTLY_ON_SEMI_OPEN_FILE_NEXT_TO_OPEN_PENALTY, "DIRECTLY_ON_SEMI_OPEN_FILE_NEXT_TO_OPEN_PENALTY,");
    print_entry(DIRECTLY_ON_SEMI_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY, "DIRECTLY_ON_SEMI_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY,");
    print_entry(NEXT_TO_SEMI_OPEN_FILE_PENALTY, "NEXT_TO_SEMI_OPEN_FILE_PENALTY,");
    print_range(NEXT_TO_OPEN_DIAGONAL_PENALTY_START, NEXT_TO_OPEN_DIAGONAL_PENALTY_END, "NEXT_TO_OPEN_DIAGONAL_PENALTY values", "NEXT_TO_OPEN_DIAGONAL_PENALTY_START");
    print_range(MOBILITY_START, MOBILITY_END, "MOBILITY values", "MOBILITY_START");
    print_range(ROOK_BEHIND_FREE_PAWN_START, ROOK_BEHIND_FREE_PAWN_END, "ROOK_BEHIND_FREE_PAWN values", "ROOK_BEHIND_FREE_PAWN_START");
    print_entry(ROOK_ON_OPEN_FILE, "ROOK_ON_OPEN_FILE,");
    print_entry(ROOK_ON_SEMI_OPEN_FILE, "ROOK_ON_SEMI_OPEN_FILE,");
    print_entry(CONNECTED_ROOKS, "CONNECTED_ROOKS,");
    print_entry(BISHOP_PAIR, "BISHOP_PAIR,");
    print_entry(BAD_BISHOP_BLOCKED, "BAD_BISHOP_BLOCKED,");
    print_entry(BAD_BISHOP_UNBLOCKED, "BAD_BISHOP_UNBLOCKED,");
    print_entry(TRAPPED_BISHOP, "TRAPPED_BISHOP,");
    print_entry(TRAPPED_KNIGHT, "TRAPPED_KNIGHT,");
    print_entry(FIANCHETTO_BISHOP, "FIANCHETTO_BISHOP,");
    print_entry(BROKEN_FIANCHETTO, "BROKEN_FIANCHETTO,");
    print_entry(BISHOP_OUTPOST_NO_OPPOSITE_BISHOP, "BISHOP_OUTPOST_NO_OPPOSITE_BISHOP,");
    print_entry(BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP, "BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP,");
    print_entry(KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP, "KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP,");
    print_entry(KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP, "KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP,");

    std::cout << "};\n";
}