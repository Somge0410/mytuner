#pragma once

#include <string>
#include <vector>
#include <array>
// Include the chess library used by the tuner
#include "board.h" 

#include "base.h" 

// 1. Define your parameters enum
// This keeps your indices perfectly organized.
struct PstNormEntry {
    int pst_start;
    int value_param;
};

class TuneEval
{
public:
    // Every evaluation term is represented by a traced parameter.
    constexpr static bool includes_additional_score = false;

    // We set this to true because using Chess::Board directly is much faster than FEN parsing
    constexpr static bool supports_external_chess_eval = false;

    // Tuning configuration
    constexpr static bool retune_from_zero = false;  // Start with current values
    constexpr static double preferred_k = 0;  // Auto-determine K
    // Optimization and validation. Cross-entropy plus a positive L2 anchor makes
    // the fixed-feature training objective strongly convex.
    constexpr static int32_t max_epoch = 500;  // Maximum L-BFGS iterations
    constexpr static double validation_fraction = 0.10;
    constexpr static uint32_t validation_seed = 0x5EED1234U;
    // Keep this weak: rare pawn/king features still need room to move.
    constexpr static double l2_regularization = 1e-8;
    constexpr static int32_t lbfgs_history_size = 12;
    constexpr static int32_t validation_interval = 5;
    constexpr static int32_t early_stopping_patience = 20;
    constexpr static double gradient_tolerance = 1e-9;
    constexpr static double relative_loss_tolerance = 1e-12;
    constexpr static double validation_min_delta = 1e-8;
    constexpr static bool enable_qsearch = false;  // No quiescence search
    constexpr static bool filter_in_check = false;  // Skip positions in check
    
    constexpr static int32_t data_load_print_interval = 10000;

    static parameters_t get_initial_parameters();
    static EvalResult get_fen_eval_result(const std::string& fen);
    static EvalResult get_external_eval_result(const Board& board);
    static void print_parameters(parameters_t& parameters);
    static void normalize_pst(parameters_t& params);
};
