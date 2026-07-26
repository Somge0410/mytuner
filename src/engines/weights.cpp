#include "evaluation.h"
#include "pst.h"
EvaluationResult EvalWeights[PARAM_COUNT] = {
        {100, 84},      // PAWN
        { 362, 195 },     // KNIGHT
        { 381, 212 },     // BISHOP
        { 510, 397 },     // ROOK
        { 1300, 1075 },   // QUEEN
            // PAWN_PST_START+0
        { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 },
        { -23,  -6 }, { -8,  -4 }, { -11,   2 }, { -15,   0 }, { -4,  -2 }, { 13,  -3 }, { 27, -12 }, { -11, -18 },
        { -27, -12 }, { -26,  -7 }, { -10,  -9 }, { -17,  -4 }, { -8,  -7 }, { -7,  -8 }, { 9, -15 }, { -16, -21 },
        { -31,  -9 }, { -20,  -7 }, { -9, -15 }, { 3, -20 }, { 0, -16 }, { 1, -18 }, { -10, -12 }, { -40, -17 },
        { -18,  17 }, { -10,  11 }, { -11,   9 }, { -5,   0 }, { 6,  -3 }, { -2,   1 }, { -6,   7 }, { -33,   5 },
        { -41,  28 }, { -40,  19 }, { 8,  13 }, { -6,  -4 }, { 32,  -3 }, { 53,  -1 }, { 19,   8 }, { -14,  14 },
        { 62,  28 }, { 56,  21 }, { 47,  17 }, { 57,  -2 }, { 19,   3 }, { 5,  14 }, { 55,  23 }, { 6,  15 },
        { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 }, { 0,   0 },
            // KNIGHT_PST_START+0
        { -39, -25 }, { -7, -20 }, { -25,  -2 }, { -8,   3 }, { -9,  -5 }, { -2,   4 }, { -10, -20 }, { -25, -14 },
        { -7, -19 }, { -13,  -8 }, { 8,  -4 }, { 16,   4 }, { 18,   2 }, { 19,   3 }, { 11,  -8 }, { 6, -21 },
        { -1,  -8 }, { 15,   6 }, { 25,   4 }, { 21,  21 }, { 32,  19 }, { 30,   8 }, { 37,  -4 }, { -1, -14 },
        { 0,  -2 }, { 1,   9 }, { 18,  21 }, { 8,  27 }, { 22,  28 }, { 25,  14 }, { 10,   7 }, { -9,   3 },
        { 10,   2 }, { 20,  11 }, { 3,  23 }, { 37,  31 }, { 16,  32 }, { 37,  22 }, { 11,  16 }, { 24,  -7 },
        { -18, -10 }, { -4,   3 }, { 16,  19 }, { 41,  18 }, { 74,  -3 }, { 84,  -2 }, { 35,  -6 }, { -16,  -2 },
        { -70,  -3 }, { -30,   4 }, { 48,  -7 }, { 15,   6 }, { 59, -12 }, { 55, -14 }, { -3, -11 }, { -11, -22 },
        { -174,   2 }, { -42, -31 }, { -48,  -4 }, { -49,   4 }, { 12,  -9 }, { -106,  -4 }, { -125,  -6 }, { -67, -47 },
            // BISHOP_PST_START+0
        { 18, -19 }, { -5,  -3 }, { 13, -16 }, { 1,  -1 }, { 0,   0 }, { -7,  -2 }, { 6, -15 }, { 13, -18 },
        { -6,  -3 }, { 33, -16 }, { 15,  -6 }, { 14,   2 }, { 17,   4 }, { 30,  -4 }, { 41, -12 }, { 16, -17 },
        { 27, -14 }, { 33,   0 }, { 29,   4 }, { 11,  12 }, { 23,  11 }, { 36,   2 }, { 26,  -7 }, { 27, -14 },
        { -12,  -2 }, { 1,   1 }, { 9,   9 }, { 23,   5 }, { 24,   5 }, { 10,  10 }, { 10,  -3 }, { -10,  -5 },
        { -24,  13 }, { -17,  10 }, { 4,   7 }, { 7,  11 }, { 15,   8 }, { -15,   7 }, { 8,  -5 }, { -10,  -1 },
        { -23,   9 }, { -9,   6 }, { 13,   4 }, { -9,  15 }, { 21,  -1 }, { 26,   8 }, { 16,   3 }, { 12,  -2 },
        { -28,   2 }, { -4,   1 }, { -21,   5 }, { -23,   1 }, { -7,   2 }, { 13,  -4 }, { -17,   6 }, { -5, -16 },
        { -33,   0 }, { -40,  -6 }, { -23,  -5 }, { -64,   8 }, { -100,  13 }, { -45,   6 }, { -21,  -4 }, { -63,  12 },
            // ROOK_PST_START+0
        { -5,  -2 }, { -5,  -2 }, { 2,  -2 }, { 3,  -2 }, { 8,  -9 }, { 9,  -7 }, { -19,  -3 }, { 7, -28 },
        { -23,  -7 }, { -19,  -3 }, { -20,   3 }, { -9,  -3 }, { -6,  -5 }, { 5,  -8 }, { 13, -13 }, { -39,  -5 },
        { -23,  -6 }, { -22,  -1 }, { -13,  -3 }, { -4,  -4 }, { -2,  -3 }, { 10, -11 }, { 27, -20 }, { 6, -18 },
        { -29,   3 }, { -39,   7 }, { -36,  11 }, { -27,  10 }, { -22,   9 }, { -8,   2 }, { 5,  -6 }, { -10, -10 },
        { -36,  10 }, { -20,   4 }, { -9,   7 }, { 4,   1 }, { -19,   7 }, { 5,   3 }, { 18,  -5 }, { 4,  -4 },
        { -16,   8 }, { -18,  11 }, { -26,  10 }, { -4,   6 }, { 5,   2 }, { 26,  -1 }, { 40,  -4 }, { 15,  -4 },
        { -25,  14 }, { -37,  27 }, { -9,  18 }, { 27,   5 }, { -15,  12 }, { 51,  -2 }, { 49,  -1 }, { 30,  -4 },
        { 8,  10 }, { 19,   4 }, { 11,  11 }, { 22,   2 }, { 41,  -1 }, { 88, -13 }, { 55,  -7 }, { 0,   7 },
            // QUEEN_PST_START+0
        { 32, -29 }, { 28, -38 }, { 34, -38 }, { 42, -56 }, { 32, -28 }, { 21, -38 }, { 6, -30 }, { 32, -52 },
        { 15, -20 }, { 25, -22 }, { 26, -24 }, { 29, -19 }, { 33, -25 }, { 41, -50 }, { 46, -58 }, { 28, -33 },
        { 14, -38 }, { 30, -49 }, { 11,  -2 }, { 16, -19 }, { 14,  -7 }, { 29, -10 }, { 29,   1 }, { 17, -21 },
        { -12,   7 }, { -7,   4 }, { -5,  -3 }, { -15,  24 }, { 0,   8 }, { 0,   4 }, { 4,   7 }, { -3,  -3 },
        { -24,   2 }, { -32,  28 }, { -24,   7 }, { -33,  25 }, { -35,  42 }, { -24,  39 }, { -29,  32 }, { 0,   3 },
        { -29,  -3 }, { -34,  12 }, { -41,  37 }, { -24,  19 }, { -43,  65 }, { -4,  22 }, { 55, -22 }, { 12,  -8 },
        { -33,  18 }, { -48,  24 }, { -56,  39 }, { -94,  83 }, { -82,  63 }, { 18,  42 }, { -1,  36 }, { 94, -67 },
        { -37,  16 }, { -47,  29 }, { 3,   9 }, { -23,  35 }, { 17,   4 }, { 62, -15 }, { -19,   4 }, { -34,  39 },
            // KING_PST_START+0
        { -84,  -9 }, { -23,  -6 }, { -21,   1 }, { -35,   4 }, { 3, -11 }, { -39,   1 }, { 1, -14 }, { -14, -35 },
        { -1, -19 }, { -24,   4 }, { -14,  13 }, { -33,  22 }, { -20,  18 }, { -19,  15 }, { 11,   3 }, { -10,  -7 },
        { -1, -23 }, { 25,  -3 }, { -18,  16 }, { -13,  22 }, { -12,  20 }, { 5,  12 }, { 10,   6 }, { -37,   2 },
        { 7, -27 }, { 39,  -6 }, { 36,   8 }, { 4,  19 }, { 11,  17 }, { -22,  17 }, { -14,   9 }, { -76,   7 },
        { -8, -24 }, { 21,  -2 }, { 65,   5 }, { 30,  16 }, { 23,  14 }, { 28,  15 }, { 23,  11 }, { -38,   7 },
        { 45, -24 }, { 134,  -7 }, { 63,   7 }, { 90,   6 }, { 35,  16 }, { 107,  12 }, { 96,  13 }, { -8,  10 },
        { 10, -14 }, { 112,  -2 }, { 79,   4 }, { 68,   6 }, { 66,   8 }, { 113,  11 }, { 21,  21 }, { -29,  11 },
        { 72, -47 }, { 62, -13 }, { 95, -14 }, { 45,   0 }, { 104,  -7 }, { 89,  -3 }, { 51,  -4 }, { 22, -18 },
            // PASSED_PAWNS_START+0
        { -25,   3 }, { -27, -13 }, { -37,  -3 }, { -34,  10 },
        { -42,  23 }, { -14,  -4 }, { -19,  -4 }, { -31,  14 },
        { 104,  34 }, { 6,  37 }, { 9,  22 }, { -24,  41 },
        { 117,  55 }, { 111,  42 }, { 33,  48 }, { 83,  54 },
            // PROTECTED_PASSED_PAWNS_START+0
        { 15,   6 }, { 14,  21 }, { 12,  17 }, { 37,  -2 },
        { 35,  10 }, { 30,  16 }, { 27,  16 }, { 28,  15 },
        { 41,  21 }, { 80,  32 }, { 114,  20 }, { 152,  -7 },
        { 319, -70 }, { 124,  26 }, { 157,  14 }, { 270, -45 },
            // BLOCKED_FREE_PAWN_START+0
        { -5,   0 }, { 5, -10 }, { -7,  -2 }, { -1,  -1 },
        { 9,  -8 }, { -14,  -2 }, { -15,  -2 }, { 46, -13 },
        { -74, -11 }, { -15, -29 }, { -16, -22 }, { -25, -22 },
        { -96,  25 }, { 8, -36 }, { -8, -55 }, { -26, -51 },
            // CANT_REACHED_BY_ENEMY_KING_START+0
        { 4,  17 }, { 145,  12 }, { 78,  10 }, { -24,  22 },
        { 4,  44 }, { 1,  57 }, { -84,  72 }, { -115,  81 },
        { -87, 108 }, { -28, 106 }, { -122, 141 }, { -152, 144 },
        { -28,  98 }, { -85, 135 }, { -123,  72 }, { -73, 100 },
            // OWN_KING_IS_CLOSE_START+0
        { 1,   3 }, { -2,   8 }, { 34,  -8 }, { 48, -17 },
        { 12,  25 }, { 0,  30 }, { -8,  24 }, { -2,  17 },
        { -8,  52 }, { 71,  43 }, { 21,  48 }, { 65,  29 },
        { 57,  40 }, { 205,  57 }, { 134,  58 }, { -137,  72 },
            // OWN_KING_IS_FAR_START+0
        { 12, -11 }, { 10,  -6 }, { -27,   5 }, { 13,  -8 },
        { 26, -15 }, { 1,  -9 }, { 34,   1 }, { -15,   5 },
        { 3,   0 }, { -13,   0 }, { -31,  -1 }, { -35,  17 },
        { -56,   4 }, { -131,  35 }, { 36,   5 }, { -92,  14 },
            // ROOK_BEHIND_FREE_PAWN_START+0
        { 15,  -5 }, { 1,   2 }, { -17,  -8 }, { 6, -15 },
        { 36,  17 }, { 10,  18 }, { 18,   1 }, { 22,   5 },
        { 35,  44 }, { 69,   5 }, { 48,  15 }, { 132, -41 },
        { 10,  32 }, { 7,  46 }, { -9,  31 }, { -27,  65 },
            // OP_ROOK_BEHIND_FREE_PAWN_START+0
        { -15,   3 }, { 32,  30 }, { 21,  -6 }, { 83, -10 },
        { 16, -37 }, { -42,  -1 }, { 53, -11 }, { 61, -37 },
        { -40, -64 }, { -146, -12 }, { -128, -21 }, { 23, -58 },
        { 32, -92 }, { 65, -90 }, { 10, -62 }, { -28, -65 },
            // ISOLANI_START+0
        { -12,  -7 }, { -21, -16 }, { -15, -18 }, { -31, -13 },
        { -6,  -8 }, { -15, -16 }, { -30,  -9 }, { -32, -10 },
        { 9, -28 }, { -1, -31 }, { -20, -26 }, { -16, -22 },
        { 60,   1 }, { 155, -52 }, { 3,  23 }, { -46,  52 },
            // BLOCKED_ISOLANI_START+0
        { -18,  -1 }, { -19, -11 }, { -23, -13 }, { -15, -10 },
        { -9,  -2 }, { -14,  -9 }, { -22,  -5 }, { -25,  -6 },
        { -8, -19 }, { -15, -28 }, { -31, -21 }, { -34, -19 },
        { 24, -27 }, { 38, -43 }, { 11, -35 }, { -46, -16 },
            // PROTECTED_ISOLANI_START+0
        { -1,  -2 }, { -2,   1 }, { -6,   2 }, { -6,  -2 },
        { 2,  -1 }, { -7,   2 }, { 1,  -3 }, { 1,  -2 },
        { 0,   0 }, { -7,   5 }, { -4,   2 }, { 4,   0 },
        { -9,   1 }, { -19,   7 }, { -37,   7 }, { 10,  -9 },
        { -14, -14 },     // FORWARD_BLOCKED_BACKWARD
        { -10, -12 },     // FORWARD_CONTROLLED_BACKWARD
        { -7, -9 },       // FREE_TO_ADV_BACKWARD
        { -8, -25 },      // DOUBLE_PAWN_FILE_START+0
        { 4, -14 },       // DOUBLE_PAWN_FILE_START+1
        { -3, -20 },      // DOUBLE_PAWN_FILE_START+2
        { 6, -21 },       // DOUBLE_PAWN_FILE_START+3
        { -1, -16 },      // DOUBLE_PAWN_FILE_START+4
        { 3, -13 },       // DOUBLE_PAWN_FILE_START+5
        { -7, -11 },      // DOUBLE_PAWN_FILE_START+6
        { 4, -24 },       // DOUBLE_PAWN_FILE_START+7
        { -22, 0 },       // KING_FILE_OPEN_WITH_RQ
        { -33, -4 },      // KING_FILE_OPEN_WITHOUT_RQ
        { -38, 1 },       // ADJ_FILE_OPEN_WITH_RQ
        { -42, -5 },      // ADJ_FILE_OPEN_WITHOUT_RQ
        { -12, 12 },      // NO_DEF_PAWNS_ON_KING_FILE_WITH_RQ
        { 50, 3 },        // NO_DEF_PAWNS_ON_KING_FILE_WITHOUT_RQ
        { -6, 7 },        // NO_ATT_PAWNS_ON_KING_FILE_WITH_RQ
        { -15, -12 },     // NO_ATT_PAWNS_ON_KING_FILE_WITHOUT_RQ
        { -8, 2 },        // FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITH_RQ
        { 57, 5 },        // FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITHOUT_RQ
        { -2, 9 },        // FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITH_RQ
        { -19, -11 },     // FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITHOUT_RQ
        { -17, -53 },     // PARAM_588
        { -2, -2 },       // PAWNS_DEFEND_DIAGONAL_START+1
        { 21, 37 },       // PAWNS_DEFEND_DIAGONAL_START+2
        { 2, 1 }, // OTHER_DEF_COUNT_NO_PAWNS_START+0
        { -23, 79 },      // OTHER_DEF_COUNT_NO_PAWNS_START+1
        { 24, -23 },      // OTHER_DEF_COUNT_NO_PAWNS_START+2
        { 18, -2 },       // OTHER_DEF_COUNT_WITH_PAWNS_START+0
        { 6, -16 },       // OTHER_DEF_COUNT_WITH_PAWNS_START+1
        { 0, 0 },         // OTHER_DEF_COUNT_WITH_PAWNS_START+2 (retune required)
        { 62, -32 },      // RIGHT_DEF_QUEEN_SIDE_SQUARE_WITH_QB
        { -4, -9 },       // RIGHT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB
        { 45, -30 },      // LEFT_DEF_QUEEN_SIDE_SQUARE_WITH_QB
        { 80, -20 },      // LEFT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB
        { 8, -31 },       // RIGHT_DEF_KING_SIDE_SQUARE_WITH_QB
        { 16, -7 },       // RIGHT_DEF_KING_SIDE_SQUARE_WITHOUT_QB
        { 20, -5 },       // LEFT_DEF_KING_SIDE_SQUARE_WITH_QB
        { 8, -21 },       // LEFT_DEF_KING_SIDE_SQUARE_WITHOUT_QB
        { 15, -43 },      // RIGHT_DEF_CENTRAL_SQUARE_WITH_QB
        { 5, -13 },       // RIGHT_DEF_CENTRAL_SQUARE_WITHOUT_QB
        { -6, 4 },        // LEFT_DEF_CENTRAL_SQUARE_WITH_QB
        { 51, -25 },      // LEFT_DEF_CENTRAL_SQUARE_WITHOUT_QB
        { 64, -6 },       // QUEEN_SIDE_FORWARD_SQUARE
        { -21, 4 },       // CENTRAL_FORWARD_SQUARE
        { 23, -14 },      // KING_SIDE_FORWARD_SQUARE
        { 5, 0 }, // SMALL_ATTACK_COUNT_START+0
        { 14, 6 },        // SMALL_ATTACK_COUNT_START+1
        { 13, 9 },        // SMALL_ATTACK_COUNT_START+2
        { -2, 5 },        // SMALL_ATTACK_COUNT_START+3
        { -48, 32 },      // SMALL_ATTACK_COUNT_START+4
        { -104, 53 },     // SMALL_ATTACK_COUNT_START+5
        { -121, 64 },     // SMALL_ATTACK_COUNT_START+6
        { -188, 81 },     // SMALL_ATTACK_COUNT_START+7
        { 21, -17 },      // BIG_ATTACK_COUNT_START+0
        { 18, -6 },       // BIG_ATTACK_COUNT_START+1
        { 14, -7 },       // BIG_ATTACK_COUNT_START+2
        { 9, -7 },        // BIG_ATTACK_COUNT_START+3
        { -10, -1 },      // BIG_ATTACK_COUNT_START+4
        { -21, 1 },       // BIG_ATTACK_COUNT_START+5
        { -47, 9 },       // BIG_ATTACK_COUNT_START+6
        { -88, 20 },      // BIG_ATTACK_COUNT_START+7
        { -123, 33 },     // BIG_ATTACK_COUNT_START+8
        { -110, 36 },     // BIG_ATTACK_COUNT_START+9
        { -173, 63 },     // BIG_ATTACK_COUNT_START+10
        { -75, 20 },      // BIG_ATTACK_COUNT_START+11
        { -312, 118 },    // BIG_ATTACK_COUNT_START+12
        { 16, 19 },       // PIECE_ATTACKING_START+0
        { -10, 6 },       // PIECE_ATTACKING_START+1
        { -26, -4 },      // PIECE_ATTACKING_START+2
        { -23, 6 },       // PIECE_ATTACKING_START+3
        { -8, -11 },      // PIECE_ATTACKING_START+4
        { 0, 0 }, // PIECE_ATTACKING_START+5
        { 5, 0 }, // DISTINCT_PIECES_ATTACKING_START+0
        { 7, 0 }, // DISTINCT_PIECES_ATTACKING_START+1
        { -11, -1 },      // DISTINCT_PIECES_ATTACKING_START+2
        { -80, 0 },       // DISTINCT_PIECES_ATTACKING_START+3
        { -112, -5 },     // DISTINCT_PIECES_ATTACKING_START+4
        { 1, 7 }, // WEAK_PAWNS_AROUND_SMALL_KING
        { -3, 13 },       // WEAK_PAWNS_AROUND_BIG_KING
        { 36, 29 },       // OP_PAWNS_IN_SMALL_MASK
        { -36, 9 },       // OP_PAWNS_IN_BIG_MASK
        { 9, -38 },       // KING_ESCAPE_SQUARES_START+0
        { 21, -6 },       // KING_ESCAPE_SQUARES_START+1
        { 22, 17 },       // KING_ESCAPE_SQUARES_START+2
        { 79, 286 },      // KING_TROPISM_START+0
        { 124, 287 },     // KING_TROPISM_START+1
        { 170, 290 },     // KING_TROPISM_START+2
        { 191, 292 },     // KING_TROPISM_START+3
        { 213, 316 },     // KING_TROPISM_START+4
        { 251, 326 },     // KING_TROPISM_START+5
        { -8, 0 },        // MOBILITY_PENALTY
        { 6, 2 }, // MOBILITY_START+0
        { 7, 4 }, // MOBILITY_START+1
        { 4, 2 }, // MOBILITY_START+2
        { 3, 6 }, // MOBILITY_START+3
        { 63, -2 },       // ROOK_ON_OPEN_FILE
        { 30, 8 },        // ROOK_ON_SEMI_OPEN_FILE
        { 4, -2 },        // CONNECTED_ROOKS
        { 57, 69 },       // BISHOP_PAIR
        { -13, -14 },     // BAD_BISHOP_BLOCKED
        { -5, -2 },       // BAD_BISHOP_UNBLOCKED
        { -266, -16 },    // TRAPPED_BISHOP
        { -143, -107 },   // TRAPPED_KNIGHT
        { 26, 0 },        // FIANCHETTO_BISHOP
        { 1, 11 },        // BROKEN_FIANCHETTO
        { 72, 3 },        // BISHOP_OUTPOST_NO_OPPOSITE_BISHOP
        { 76, 6 },        // BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP
        { 59, 19 },       // KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP
        { 45, 28 },       // KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP
            // CONNECTED_PASSED_PAWNS_START+0 (retune required)
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
            // PIECE_SUPPORTED_PASSED_PAWNS_START+0 (retune required)
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
            // SAFE_ADVANCE_PASSED_PAWNS_START+0 (retune required)
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
            // CLEAR_PATH_PASSED_PAWNS_START+0 (retune required)
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
        { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
};
