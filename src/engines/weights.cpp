#include "evaluation.h"
#include "pst.h"

EvaluationResult EvalWeights[PARAM_COUNT1] = {
        {100,178},      // PAWN
        {383,499},      // KNIGHT
        {418,522},      // BISHOP
        {501,891},      // ROOK
        {1093,1688},    // QUEEN
        {0,0},  // PAWN_PST_START+0
        {0,0},  // PAWN_PST_START+1
        {0,0},  // PAWN_PST_START+2
        {0,0},  // PAWN_PST_START+3
        {0,0},  // PAWN_PST_START+4
        {0,0},  // PAWN_PST_START+5
        {0,0},  // PAWN_PST_START+6
        {0,0},  // PAWN_PST_START+7
        {-67,26},       // PAWN_PST_START+8
        {-43,27},       // PAWN_PST_START+9
        {-47,14},       // PAWN_PST_START+10
        {-57,20},       // PAWN_PST_START+11
        {-37,25},       // PAWN_PST_START+12
        {-22,13},       // PAWN_PST_START+13
        {0,12}, // PAWN_PST_START+14
        {-45,5},        // PAWN_PST_START+15
        {-67,21},       // PAWN_PST_START+16
        {-43,23},       // PAWN_PST_START+17
        {-43,6},        // PAWN_PST_START+18
        {-42,18},       // PAWN_PST_START+19
        {-28,11},       // PAWN_PST_START+20
        {-38,8},        // PAWN_PST_START+21
        {-9,13},        // PAWN_PST_START+22
        {-38,4},        // PAWN_PST_START+23
        {-66,27},       // PAWN_PST_START+24
        {-39,25},       // PAWN_PST_START+25
        {-40,7},        // PAWN_PST_START+26
        {-24,4},        // PAWN_PST_START+27
        {-24,2},        // PAWN_PST_START+28
        {-31,4},        // PAWN_PST_START+29
        {-23,15},       // PAWN_PST_START+30
        {-45,8},        // PAWN_PST_START+31
        {-56,51},       // PAWN_PST_START+32
        {-31,41},       // PAWN_PST_START+33
        {-28,22},       // PAWN_PST_START+34
        {-26,13},       // PAWN_PST_START+35
        {-5,4}, // PAWN_PST_START+36
        {-14,7},        // PAWN_PST_START+37
        {-9,25},        // PAWN_PST_START+38
        {-33,27},       // PAWN_PST_START+39
        {-40,118},      // PAWN_PST_START+40
        {-27,126},      // PAWN_PST_START+41
        {6,93}, // PAWN_PST_START+42
        {11,72},        // PAWN_PST_START+43
        {14,64},        // PAWN_PST_START+44
        {36,49},        // PAWN_PST_START+45
        {16,95},        // PAWN_PST_START+46
        {-26,93},       // PAWN_PST_START+47
        {40,175},       // PAWN_PST_START+48
        {61,167},       // PAWN_PST_START+49
        {39,167},       // PAWN_PST_START+50
        {68,120},       // PAWN_PST_START+51
        {52,117},       // PAWN_PST_START+52
        {35,127},       // PAWN_PST_START+53
        {-31,171},      // PAWN_PST_START+54
        {-56,184},      // PAWN_PST_START+55
        {0,0},  // PAWN_PST_START+56
        {0,0},  // PAWN_PST_START+57
        {0,0},  // PAWN_PST_START+58
        {0,0},  // PAWN_PST_START+59
        {0,0},  // PAWN_PST_START+60
        {0,0},  // PAWN_PST_START+61
        {0,0},  // PAWN_PST_START+62
        {0,0},  // PAWN_PST_START+63
        {-153,46},      // KNIGHT_PST_START+0
        {-101,34},      // KNIGHT_PST_START+1
        {-114,62},      // KNIGHT_PST_START+2
        {-100,66},      // KNIGHT_PST_START+3
        {-95,66},       // KNIGHT_PST_START+4
        {-83,56},       // KNIGHT_PST_START+5
        {-99,41},       // KNIGHT_PST_START+6
        {-124,35},      // KNIGHT_PST_START+7
        {-111,53},      // KNIGHT_PST_START+8
        {-100,69},      // KNIGHT_PST_START+9
        {-83,78},       // KNIGHT_PST_START+10
        {-72,81},       // KNIGHT_PST_START+11
        {-71,80},       // KNIGHT_PST_START+12
        {-69,75},       // KNIGHT_PST_START+13
        {-81,60},       // KNIGHT_PST_START+14
        {-85,64},       // KNIGHT_PST_START+15
        {-98,62},       // KNIGHT_PST_START+16
        {-76,80},       // KNIGHT_PST_START+17
        {-63,90},       // KNIGHT_PST_START+18
        {-60,103},      // KNIGHT_PST_START+19
        {-49,101},      // KNIGHT_PST_START+20
        {-59,86},       // KNIGHT_PST_START+21
        {-55,75},       // KNIGHT_PST_START+22
        {-83,64},       // KNIGHT_PST_START+23
        {-80,78},       // KNIGHT_PST_START+24
        {-65,87},       // KNIGHT_PST_START+25
        {-51,109},      // KNIGHT_PST_START+26
        {-49,109},      // KNIGHT_PST_START+27
        {-41,112},      // KNIGHT_PST_START+28
        {-45,103},      // KNIGHT_PST_START+29
        {-47,89},       // KNIGHT_PST_START+30
        {-70,69},       // KNIGHT_PST_START+31
        {-67,76},       // KNIGHT_PST_START+32
        {-54,96},       // KNIGHT_PST_START+33
        {-30,107},      // KNIGHT_PST_START+34
        {-9,109},       // KNIGHT_PST_START+35
        {-27,110},      // KNIGHT_PST_START+36
        {-2,104},       // KNIGHT_PST_START+37
        {-44,95},       // KNIGHT_PST_START+38
        {-34,69},       // KNIGHT_PST_START+39
        {-63,66},       // KNIGHT_PST_START+40
        {-29,80},       // KNIGHT_PST_START+41
        {-12,96},       // KNIGHT_PST_START+42
        {-1,96},        // KNIGHT_PST_START+43
        {36,81},        // KNIGHT_PST_START+44
        {37,76},        // KNIGHT_PST_START+45
        {-7,70},        // KNIGHT_PST_START+46
        {-37,56},       // KNIGHT_PST_START+47
        {-78,50},       // KNIGHT_PST_START+48
        {-61,70},       // KNIGHT_PST_START+49
        {-35,78},       // KNIGHT_PST_START+50
        {-19,77},       // KNIGHT_PST_START+51
        {-34,70},       // KNIGHT_PST_START+52
        {27,55},        // KNIGHT_PST_START+53
        {-61,67},       // KNIGHT_PST_START+54
        {-39,35},       // KNIGHT_PST_START+55
        {-212,1},       // KNIGHT_PST_START+56
        {-175,57},      // KNIGHT_PST_START+57
        {-112,70},      // KNIGHT_PST_START+58
        {-81,63},       // KNIGHT_PST_START+59
        {-49,65},       // KNIGHT_PST_START+60
        {-106,43},      // KNIGHT_PST_START+61
        {-153,62},      // KNIGHT_PST_START+62
        {-155,-22},     // KNIGHT_PST_START+63
        {4,82}, // BISHOP_PST_START+0
        {24,99},        // BISHOP_PST_START+1
        {8,80}, // BISHOP_PST_START+2
        {0,102},        // BISHOP_PST_START+3
        {4,98}, // BISHOP_PST_START+4
        {3,99}, // BISHOP_PST_START+5
        {28,84},        // BISHOP_PST_START+6
        {14,69},        // BISHOP_PST_START+7
        {25,98},        // BISHOP_PST_START+8
        {27,99},        // BISHOP_PST_START+9
        {37,97},        // BISHOP_PST_START+10
        {16,112},       // BISHOP_PST_START+11
        {24,114},       // BISHOP_PST_START+12
        {36,102},       // BISHOP_PST_START+13
        {42,104},       // BISHOP_PST_START+14
        {30,79},        // BISHOP_PST_START+15
        {23,104},       // BISHOP_PST_START+16
        {30,114},       // BISHOP_PST_START+17
        {30,121},       // BISHOP_PST_START+18
        {33,121},       // BISHOP_PST_START+19
        {34,125},       // BISHOP_PST_START+20
        {29,121},       // BISHOP_PST_START+21
        {32,105},       // BISHOP_PST_START+22
        {35,94},        // BISHOP_PST_START+23
        {13,104},       // BISHOP_PST_START+24
        {26,121},       // BISHOP_PST_START+25
        {32,129},       // BISHOP_PST_START+26
        {52,125},       // BISHOP_PST_START+27
        {49,125},       // BISHOP_PST_START+28
        {34,124},       // BISHOP_PST_START+29
        {26,118},       // BISHOP_PST_START+30
        {21,94},        // BISHOP_PST_START+31
        {19,108},       // BISHOP_PST_START+32
        {33,124},       // BISHOP_PST_START+33
        {55,118},       // BISHOP_PST_START+34
        {65,131},       // BISHOP_PST_START+35
        {62,124},       // BISHOP_PST_START+36
        {58,122},       // BISHOP_PST_START+37
        {34,121},       // BISHOP_PST_START+38
        {20,108},       // BISHOP_PST_START+39
        {27,113},       // BISHOP_PST_START+40
        {51,107},       // BISHOP_PST_START+41
        {51,118},       // BISHOP_PST_START+42
        {74,107},       // BISHOP_PST_START+43
        {61,112},       // BISHOP_PST_START+44
        {92,113},       // BISHOP_PST_START+45
        {69,106},       // BISHOP_PST_START+46
        {57,106},       // BISHOP_PST_START+47
        {18,85},        // BISHOP_PST_START+48
        {41,103},       // BISHOP_PST_START+49
        {35,107},       // BISHOP_PST_START+50
        {18,110},       // BISHOP_PST_START+51
        {47,101},       // BISHOP_PST_START+52
        {47,99},        // BISHOP_PST_START+53
        {38,107},       // BISHOP_PST_START+54
        {27,83},        // BISHOP_PST_START+55
        {2,97}, // BISHOP_PST_START+56
        {-16,108},      // BISHOP_PST_START+57
        {-6,106},       // BISHOP_PST_START+58
        {-46,119},      // BISHOP_PST_START+59
        {-35,112},      // BISHOP_PST_START+60
        {-18,103},      // BISHOP_PST_START+61
        {13,96},        // BISHOP_PST_START+62
        {-25,95},       // BISHOP_PST_START+63
        {24,186},       // ROOK_PST_START+0
        {25,195},       // ROOK_PST_START+1
        {34,202},       // ROOK_PST_START+2
        {40,201},       // ROOK_PST_START+3
        {44,193},       // ROOK_PST_START+4
        {34,189},       // ROOK_PST_START+5
        {48,184},       // ROOK_PST_START+6
        {25,178},       // ROOK_PST_START+7
        {5,190},        // ROOK_PST_START+8
        {17,193},       // ROOK_PST_START+9
        {32,194},       // ROOK_PST_START+10
        {28,196},       // ROOK_PST_START+11
        {33,188},       // ROOK_PST_START+12
        {35,184},       // ROOK_PST_START+13
        {51,175},       // ROOK_PST_START+14
        {22,180},       // ROOK_PST_START+15
        {8,195},        // ROOK_PST_START+16
        {17,195},       // ROOK_PST_START+17
        {26,194},       // ROOK_PST_START+18
        {25,199},       // ROOK_PST_START+19
        {30,194},       // ROOK_PST_START+20
        {28,187},       // ROOK_PST_START+21
        {61,167},       // ROOK_PST_START+22
        {40,168},       // ROOK_PST_START+23
        {15,199},       // ROOK_PST_START+24
        {17,204},       // ROOK_PST_START+25
        {27,206},       // ROOK_PST_START+26
        {39,205},       // ROOK_PST_START+27
        {39,200},       // ROOK_PST_START+28
        {24,198},       // ROOK_PST_START+29
        {47,186},       // ROOK_PST_START+30
        {39,181},       // ROOK_PST_START+31
        {33,206},       // ROOK_PST_START+32
        {47,205},       // ROOK_PST_START+33
        {49,213},       // ROOK_PST_START+34
        {57,210},       // ROOK_PST_START+35
        {63,195},       // ROOK_PST_START+36
        {63,189},       // ROOK_PST_START+37
        {72,186},       // ROOK_PST_START+38
        {74,180},       // ROOK_PST_START+39
        {49,204},       // ROOK_PST_START+40
        {69,207},       // ROOK_PST_START+41
        {71,208},       // ROOK_PST_START+42
        {75,206},       // ROOK_PST_START+43
        {102,193},      // ROOK_PST_START+44
        {103,187},      // ROOK_PST_START+45
        {140,179},      // ROOK_PST_START+46
        {118,174},      // ROOK_PST_START+47
        {69,203},       // ROOK_PST_START+48
        {69,215},       // ROOK_PST_START+49
        {87,219},       // ROOK_PST_START+50
        {107,210},      // ROOK_PST_START+51
        {93,210},       // ROOK_PST_START+52
        {122,196},      // ROOK_PST_START+53
        {108,193},      // ROOK_PST_START+54
        {138,179},      // ROOK_PST_START+55
        {88,203},       // ROOK_PST_START+56
        {79,210},       // ROOK_PST_START+57
        {87,219},       // ROOK_PST_START+58
        {91,215},       // ROOK_PST_START+59
        {110,206},      // ROOK_PST_START+60
        {125,196},      // ROOK_PST_START+61
        {108,198},      // ROOK_PST_START+62
        {128,193},      // ROOK_PST_START+63
        {56,181},       // QUEEN_PST_START+0
        {45,190},       // QUEEN_PST_START+1
        {52,193},       // QUEEN_PST_START+2
        {67,185},       // QUEEN_PST_START+3
        {59,189},       // QUEEN_PST_START+4
        {46,188},       // QUEEN_PST_START+5
        {70,155},       // QUEEN_PST_START+6
        {64,151},       // QUEEN_PST_START+7
        {58,189},       // QUEEN_PST_START+8
        {63,193},       // QUEEN_PST_START+9
        {73,190},       // QUEEN_PST_START+10
        {72,200},       // QUEEN_PST_START+11
        {70,203},       // QUEEN_PST_START+12
        {79,177},       // QUEEN_PST_START+13
        {85,149},       // QUEEN_PST_START+14
        {97,117},       // QUEEN_PST_START+15
        {59,194},       // QUEEN_PST_START+16
        {66,210},       // QUEEN_PST_START+17
        {61,233},       // QUEEN_PST_START+18
        {60,232},       // QUEEN_PST_START+19
        {63,235},       // QUEEN_PST_START+20
        {70,226},       // QUEEN_PST_START+21
        {82,204},       // QUEEN_PST_START+22
        {76,192},       // QUEEN_PST_START+23
        {62,205},       // QUEEN_PST_START+24
        {59,234},       // QUEEN_PST_START+25
        {58,244},       // QUEEN_PST_START+26
        {67,263},       // QUEEN_PST_START+27
        {65,261},       // QUEEN_PST_START+28
        {65,252},       // QUEEN_PST_START+29
        {76,232},       // QUEEN_PST_START+30
        {79,219},       // QUEEN_PST_START+31
        {59,210},       // QUEEN_PST_START+32
        {64,232},       // QUEEN_PST_START+33
        {68,246},       // QUEEN_PST_START+34
        {67,270},       // QUEEN_PST_START+35
        {69,282},       // QUEEN_PST_START+36
        {82,267},       // QUEEN_PST_START+37
        {82,253},       // QUEEN_PST_START+38
        {88,231},       // QUEEN_PST_START+39
        {76,200},       // QUEEN_PST_START+40
        {74,217},       // QUEEN_PST_START+41
        {72,258},       // QUEEN_PST_START+42
        {88,260},       // QUEEN_PST_START+43
        {93,272},       // QUEEN_PST_START+44
        {135,252},      // QUEEN_PST_START+45
        {135,215},      // QUEEN_PST_START+46
        {132,203},      // QUEEN_PST_START+47
        {75,190},       // QUEEN_PST_START+48
        {53,232},       // QUEEN_PST_START+49
        {60,264},       // QUEEN_PST_START+50
        {54,281},       // QUEEN_PST_START+51
        {60,297},       // QUEEN_PST_START+52
        {96,258},       // QUEEN_PST_START+53
        {76,241},       // QUEEN_PST_START+54
        {119,217},      // QUEEN_PST_START+55
        {38,226},       // QUEEN_PST_START+56
        {47,237},       // QUEEN_PST_START+57
        {80,251},       // QUEEN_PST_START+58
        {112,238},      // QUEEN_PST_START+59
        {110,237},      // QUEEN_PST_START+60
        {123,223},      // QUEEN_PST_START+61
        {138,182},      // QUEEN_PST_START+62
        {81,216},       // QUEEN_PST_START+63
        {65,-72},       // KING_PST_START+0
        {88,-54},       // KING_PST_START+1
        {62,-34},       // KING_PST_START+2
        {-36,-16},      // KING_PST_START+3
        {27,-41},       // KING_PST_START+4
        {-12,-18},      // KING_PST_START+5
        {69,-44},       // KING_PST_START+6
        {70,-72},       // KING_PST_START+7
        {69,-38},       // KING_PST_START+8
        {29,-12},       // KING_PST_START+9
        {16,0}, // KING_PST_START+10
        {-18,11},       // KING_PST_START+11
        {-20,14},       // KING_PST_START+12
        {-2,5}, // KING_PST_START+13
        {45,-13},       // KING_PST_START+14
        {54,-31},       // KING_PST_START+15
        {-17,-17},      // KING_PST_START+16
        {-2,5}, // KING_PST_START+17
        {-57,26},       // KING_PST_START+18
        {-70,38},       // KING_PST_START+19
        {-63,37},       // KING_PST_START+20
        {-61,29},       // KING_PST_START+21
        {-17,10},       // KING_PST_START+22
        {-33,-1},       // KING_PST_START+23
        {-60,-7},       // KING_PST_START+24
        {-69,23},       // KING_PST_START+25
        {-99,46},       // KING_PST_START+26
        {-125,60},      // KING_PST_START+27
        {-122,60},      // KING_PST_START+28
        {-86,47},       // KING_PST_START+29
        {-90,36},       // KING_PST_START+30
        {-113,20},      // KING_PST_START+31
        {-66,4},        // KING_PST_START+32
        {-76,38},       // KING_PST_START+33
        {-90,55},       // KING_PST_START+34
        {-135,68},      // KING_PST_START+35
        {-122,67},      // KING_PST_START+36
        {-85,61},       // KING_PST_START+37
        {-84,53},       // KING_PST_START+38
        {-107,28},      // KING_PST_START+39
        {-100,15},      // KING_PST_START+40
        {3,32}, // KING_PST_START+41
        {-63,51},       // KING_PST_START+42
        {-81,61},       // KING_PST_START+43
        {-43,61},       // KING_PST_START+44
        {30,53},        // KING_PST_START+45
        {12,52},        // KING_PST_START+46
        {-24,24},       // KING_PST_START+47
        {-79,-1},       // KING_PST_START+48
        {-39,27},       // KING_PST_START+49
        {-80,40},       // KING_PST_START+50
        {20,22},        // KING_PST_START+51
        {-27,42},       // KING_PST_START+52
        {-22,53},       // KING_PST_START+53
        {18,43},        // KING_PST_START+54
        {0,13}, // KING_PST_START+55
        {47,-94},       // KING_PST_START+56
        {30,-47},       // KING_PST_START+57
        {51,-35},       // KING_PST_START+58
        {-81,13},       // KING_PST_START+59
        {-38,-4},       // KING_PST_START+60
        {21,-3},        // KING_PST_START+61
        {81,-13},       // KING_PST_START+62
        {195,-118},     // KING_PST_START+63
            //PAWN EVAL
            // PASSED PST values
        { passed_pawns_MG[0], passed_pawns_EG[0] },    // PASSED_PAWNS_START
        { passed_pawns_MG[1], passed_pawns_EG[1] },    // PASSED_PAWNS_START+1
        { passed_pawns_MG[2], passed_pawns_EG[2] },    // PASSED_PAWNS_START+2
        { passed_pawns_MG[3], passed_pawns_EG[3] },    // PASSED_PAWNS_START+3
        { passed_pawns_MG[4], passed_pawns_EG[4] },    // PASSED_PAWNS_START+4
        { passed_pawns_MG[5], passed_pawns_EG[5] },    // PASSED_PAWNS_START+5
        { passed_pawns_MG[6], passed_pawns_EG[6] },    // PASSED_PAWNS_START+6
        { passed_pawns_MG[7], passed_pawns_EG[7] },    // PASSED_PAWNS_START+7
        { passed_pawns_MG[8], passed_pawns_EG[8] },    // PASSED_PAWNS_START+8
        { passed_pawns_MG[9], passed_pawns_EG[9] },    // PASSED_PAWNS_START+9
        { passed_pawns_MG[10], passed_pawns_EG[10] },  // PASSED_PAWNS_START+10
        { passed_pawns_MG[11], passed_pawns_EG[11] },  // PASSED_PAWNS_START+11
        { passed_pawns_MG[12], passed_pawns_EG[12] },  // PASSED_PAWNS_START+12
        { passed_pawns_MG[13], passed_pawns_EG[13] },  // PASSED_PAWNS_START+13
        { passed_pawns_MG[14], passed_pawns_EG[14] },  // PASSED_PAWNS_START+14
        { passed_pawns_MG[15], passed_pawns_EG[15] },  // PASSED_PAWNS_START+15
        { passed_pawns_MG[16], passed_pawns_EG[16] },  // PASSED_PAWNS_START+16
        { passed_pawns_MG[17], passed_pawns_EG[17] },  // PASSED_PAWNS_START+17
        { passed_pawns_MG[18], passed_pawns_EG[18] },  // PASSED_PAWNS_START+18
        { passed_pawns_MG[19], passed_pawns_EG[19] },  // PASSED_PAWNS_START+19
        { passed_pawns_MG[20], passed_pawns_EG[20] },  // PASSED_PAWNS_START+20
        { passed_pawns_MG[21], passed_pawns_EG[21] },  // PASSED_PAWNS_START+21
        { passed_pawns_MG[22], passed_pawns_EG[22] },  // PASSED_PAWNS_START+22
        { passed_pawns_MG[23], passed_pawns_EG[23] },  // PASSED_PAWNS_START+23
        { passed_pawns_MG[24], passed_pawns_EG[24] },  // PASSED_PAWNS_START+24
        { passed_pawns_MG[25], passed_pawns_EG[25] },  // PASSED_PAWNS_START+25
        { passed_pawns_MG[26], passed_pawns_EG[26] },  // PASSED_PAWNS_START+26
        { passed_pawns_MG[27], passed_pawns_EG[27] },  // PASSED_PAWNS_START+27
        { passed_pawns_MG[28], passed_pawns_EG[28] },  // PASSED_PAWNS_START+28
        { passed_pawns_MG[29], passed_pawns_EG[29] },  // PASSED_PAWNS_START+29
        { passed_pawns_MG[30], passed_pawns_EG[30] },  // PASSED_PAWNS_START+30
        { passed_pawns_MG[31], passed_pawns_EG[31] },  // PASSED_PAWNS_START+31
        { passed_pawns_MG[32], passed_pawns_EG[32] },  // PASSED_PAWNS_START+32
        { passed_pawns_MG[33], passed_pawns_EG[33] },  // PASSED_PAWNS_START+33
        { passed_pawns_MG[34], passed_pawns_EG[34] },  // PASSED_PAWNS_START+34
        { passed_pawns_MG[35], passed_pawns_EG[35] },  // PASSED_PAWNS_START+35
        { passed_pawns_MG[36], passed_pawns_EG[36] },  // PASSED_PAWNS_START+36
        { passed_pawns_MG[37], passed_pawns_EG[37] },  // PASSED_PAWNS_START+37
        { passed_pawns_MG[38], passed_pawns_EG[38] },  // PASSED_PAWNS_START+38
        { passed_pawns_MG[39], passed_pawns_EG[39] },  // PASSED_PAWNS_START+39
        { passed_pawns_MG[40], passed_pawns_EG[40] },  // PASSED_PAWNS_START+40
        { passed_pawns_MG[41], passed_pawns_EG[41] },  // PASSED_PAWNS_START+41
        { passed_pawns_MG[42], passed_pawns_EG[42] },  // PASSED_PAWNS_START+42
        { passed_pawns_MG[43], passed_pawns_EG[43] },  // PASSED_PAWNS_START+43
        { passed_pawns_MG[44], passed_pawns_EG[44] },  // PASSED_PAWNS_START+44
        { passed_pawns_MG[45], passed_pawns_EG[45] },  // PASSED_PAWNS_START+45
        { passed_pawns_MG[46], passed_pawns_EG[46] },  // PASSED_PAWNS_START+46
        { passed_pawns_MG[47], passed_pawns_EG[47] },  // PASSED_PAWNS_START+47
        { passed_pawns_MG[48], passed_pawns_EG[48] },  // PASSED_PAWNS_START+48
        { passed_pawns_MG[49], passed_pawns_EG[49] },  // PASSED_PAWNS_START+49
        { passed_pawns_MG[50], passed_pawns_EG[50] },  // PASSED_PAWNS_START+50
        { passed_pawns_MG[51], passed_pawns_EG[51] },  // PASSED_PAWNS_START+51
        { passed_pawns_MG[52], passed_pawns_EG[52] },  // PASSED_PAWNS_START+52
        { passed_pawns_MG[53], passed_pawns_EG[53] },  // PASSED_PAWNS_START+53
        { passed_pawns_MG[54], passed_pawns_EG[54] },  // PASSED_PAWNS_START+54
        { passed_pawns_MG[55], passed_pawns_EG[55] },  // PASSED_PAWNS_START+55
        { passed_pawns_MG[56], passed_pawns_EG[56] },  // PASSED_PAWNS_START+56
        { passed_pawns_MG[57], passed_pawns_EG[57] },  // PASSED_PAWNS_START+57
        { passed_pawns_MG[58], passed_pawns_EG[58] },  // PASSED_PAWNS_START+58
        { passed_pawns_MG[59], passed_pawns_EG[59] },  // PASSED_PAWNS_START+59
        { passed_pawns_MG[60], passed_pawns_EG[60] },  // PASSED_PAWNS_START+60
        { passed_pawns_MG[61], passed_pawns_EG[61] },  // PASSED_PAWNS_START+61
        { passed_pawns_MG[62], passed_pawns_EG[62] },  // PASSED_PAWNS_START+62
        { passed_pawns_MG[63], passed_pawns_EG[63] },  // PASSED_PAWNS_START+63
            // Knight PST values
        { 0,0 },    // ISOLANI_START
        { 0,0 },    // ISOLANI_START+1
        { 0,0 },    // ISOLANI_START+2
        { 0,0 },    // ISOLANI_START+3
        { 0,0 },    // ISOLANI_START+4
        { 0,0 },    // ISOLANI_START+5
        { 0,0 },    // ISOLANI_START+6
        { 0,0 },    // ISOLANI_START+7
        { -9,-8 },          // ISOLANI_START+8
        { -23,-17 },        // ISOLANI_START+9
        { -15,-19 },        // ISOLANI_START+10
        { -38,-11 },        // ISOLANI_START+11
        { -44,-17 },        // ISOLANI_START+12
        { -21,-10 },        // ISOLANI_START+13
        { -11,-20 },        // ISOLANI_START+14
        { -13,6 },          // ISOLANI_START+15
        { -9,-8 },          // ISOLANI_START+16
        { -17,-28 },        // ISOLANI_START+17
        { -33,-19 },        // ISOLANI_START+18
        { -27,-25 },        // ISOLANI_START+19
        { -36,-21 },        // ISOLANI_START+20
        { -26,-11 },        // ISOLANI_START+21
        { -29,-21 },        // ISOLANI_START+22
        { -23,4 },          // ISOLANI_START+23
        { -1,-9 },          // ISOLANI_START+24
        { -14,-26 },        // ISOLANI_START+25
        { -34,-18 },        // ISOLANI_START+26
        { -36,-23 },        // ISOLANI_START+27
        { -31,-25 },        // ISOLANI_START+28
        { -27,-10 },        // ISOLANI_START+29
        { -15,-24 },        // ISOLANI_START+30
        { -3,-3 },          // ISOLANI_START+31
        { 16,-11 },         // ISOLANI_START+32
        { -3,-38 },         // ISOLANI_START+33
        { -7,-22 },         // ISOLANI_START+34
        { -4,-17 },         // ISOLANI_START+35
        { -21,-20 },        // ISOLANI_START+36
        { 0,-21 },          // ISOLANI_START+37
        { 32,-44 },         // ISOLANI_START+38
        { 21,-1 },          // ISOLANI_START+39
        { 73,91 },          // ISOLANI_START+40
        { 62,34 },          // ISOLANI_START+41
        { 26,49 },          // ISOLANI_START+42
        { 32,49 },          // ISOLANI_START+43
        { 4,58 },   // ISOLANI_START+44
        { 27,26 },          // ISOLANI_START+45
        { 69,2 },   // ISOLANI_START+46
        { 62,30 },          // ISOLANI_START+47
        { 0,0 },    // ISOLANI_START+48
        { 0,0 },    // ISOLANI_START+49
        { 0,0 },    // ISOLANI_START+50
        { 0,0 },    // ISOLANI_START+51
        { 0,0 },    // ISOLANI_START+52
        { 0,0 },    // ISOLANI_START+53
        { 0,0 },    // ISOLANI_START+54
        { 0,0 },    // ISOLANI_START+55
        { 0,0 },    // ISOLANI_START+56
        { 0,0 },    // ISOLANI_START+57
        { 0,0 },    // ISOLANI_START+58
        { 0,0 },    // ISOLANI_START+59
        { 0,0 },    // ISOLANI_START+60
        { 0,0 },    // ISOLANI_START+61
        { 0,0 },    // ISOLANI_START+62
        { 0,0 },    // ISOLANI_START+63
            // Knight PST values
        { 0,0 },    // BLOCKED_ISOLANI_START
        { 0,0 },    // BLOCKED_ISOLANI_START+1
        { 0,0 },    // BLOCKED_ISOLANI_START+2
        { 0,0 },    // BLOCKED_ISOLANI_START+3
        { 0,0 },    // BLOCKED_ISOLANI_START+4
        { 0,0 },    // BLOCKED_ISOLANI_START+5
        { 0,0 },    // BLOCKED_ISOLANI_START+6
        { 0,0 },    // BLOCKED_ISOLANI_START+7
        { -12,-20 },        // BLOCKED_ISOLANI_START+8
        { -27,-18 },        // BLOCKED_ISOLANI_START+9
        { -22,-19 },        // BLOCKED_ISOLANI_START+10
        { -50,-5 },         // BLOCKED_ISOLANI_START+11
        { -37,-19 },        // BLOCKED_ISOLANI_START+12
        { -31,-2 },         // BLOCKED_ISOLANI_START+13
        { -28,-10 },        // BLOCKED_ISOLANI_START+14
        { -21,7 },          // BLOCKED_ISOLANI_START+15
        { -13,8 },          // BLOCKED_ISOLANI_START+16
        { -16,-9 },         // BLOCKED_ISOLANI_START+17
        { -35,-5 },         // BLOCKED_ISOLANI_START+18
        { -6,-8 },          // BLOCKED_ISOLANI_START+19
        { -18,-17 },        // BLOCKED_ISOLANI_START+20
        { -24,-6 },         // BLOCKED_ISOLANI_START+21
        { -58,-5 },         // BLOCKED_ISOLANI_START+22
        { -27,11 },         // BLOCKED_ISOLANI_START+23
        { -7,6 },   // BLOCKED_ISOLANI_START+24
        { -18,-7 },         // BLOCKED_ISOLANI_START+25
        { -33,-3 },         // BLOCKED_ISOLANI_START+26
        { -20,-17 },        // BLOCKED_ISOLANI_START+27
        { -19,-15 },        // BLOCKED_ISOLANI_START+28
        { -24,-2 },         // BLOCKED_ISOLANI_START+29
        { -27,-3 },         // BLOCKED_ISOLANI_START+30
        { -8,12 },          // BLOCKED_ISOLANI_START+31
        { -6,-3 },          // BLOCKED_ISOLANI_START+32
        { -13,-26 },        // BLOCKED_ISOLANI_START+33
        { -24,-20 },        // BLOCKED_ISOLANI_START+34
        { -24,-15 },        // BLOCKED_ISOLANI_START+35
        { -28,-16 },        // BLOCKED_ISOLANI_START+36
        { -15,-17 },        // BLOCKED_ISOLANI_START+37
        { 6,-26 },          // BLOCKED_ISOLANI_START+38
        { -14,-2 },         // BLOCKED_ISOLANI_START+39
        { 19,-37 },         // BLOCKED_ISOLANI_START+40
        { 7,-72 },          // BLOCKED_ISOLANI_START+41
        { -18,-36 },        // BLOCKED_ISOLANI_START+42
        { -7,-38 },         // BLOCKED_ISOLANI_START+43
        { -32,-29 },        // BLOCKED_ISOLANI_START+44
        { -14,-25 },        // BLOCKED_ISOLANI_START+45
        { 5,-32 },          // BLOCKED_ISOLANI_START+46
        { -10,-26 },        // BLOCKED_ISOLANI_START+47
        { 0,0 },    // BLOCKED_ISOLANI_START+48
        { 0,0 },    // BLOCKED_ISOLANI_START+49
        { 0,0 },    // BLOCKED_ISOLANI_START+50
        { 0,0 },    // BLOCKED_ISOLANI_START+51
        { 0,0 },    // BLOCKED_ISOLANI_START+52
        { 0,0 },    // BLOCKED_ISOLANI_START+53
        { 0,0 },    // BLOCKED_ISOLANI_START+54
        { 0,0 },    // BLOCKED_ISOLANI_START+55
        { 0,0 },    // BLOCKED_ISOLANI_START+56
        { 0,0 },    // BLOCKED_ISOLANI_START+57
        { 0,0 },    // BLOCKED_ISOLANI_START+58
        { 0,0 },    // BLOCKED_ISOLANI_START+59
        { 0,0 },    // BLOCKED_ISOLANI_START+60
        { 0,0 },    // BLOCKED_ISOLANI_START+61
        { 0,0 },    // BLOCKED_ISOLANI_START+62
        { 0,0 },    // BLOCKED_ISOLANI_START+63
        { -13,-12 },        // FORWARD_BLOCKED_BACKWARD
        { -10,-10 },        // FORWARD_CONTROLLED_BACKWARD
        { -8,-5 },          // FREE_TO_ADV_BACKWARD
            // DOUBLE_PAWN_FILE values
        { -25,-35 },        // DOUBLE_PAWN_FILE_START
        { -1,-25 },         // DOUBLE_PAWN_FILE_START+1
        { 1,-23 },          // DOUBLE_PAWN_FILE_START+2
        { 1,-18 },          // DOUBLE_PAWN_FILE_START+3
        { -5,-11 },         // DOUBLE_PAWN_FILE_START+4
        { -7,-21 },         // DOUBLE_PAWN_FILE_START+5
        { -6,-28 },         // DOUBLE_PAWN_FILE_START+6
        { -18,-43 },        // DOUBLE_PAWN_FILE_START+7
        { 0,0 },         // PAWN_SHIELD_BONUS,
        { 0,0 },        // DIRECTLY_ON_OPEN_FILE_NEXT_TO_OPEN_PENALTY,
        { 0,0 },         // DIRECTLY_ON_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY,
        { 0,0 },        // NEXT_TO_OPEN_FILE_PENALTY,
        { 0,0 },   // DIRECTLY_ON_SEMI_OPEN_FILE_NEXT_TO_OPEN_PENALTY,
        { 0,0 },          // DIRECTLY_ON_SEMI_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY,
        { 0,0 },          // NEXT_TO_SEMI_OPEN_FILE_PENALTY,
            // NEXT_TO_OPEN_DIAGONAL_PENALTY values
        { 0,0 },        // NEXT_TO_OPEN_DIAGONAL_PENALTY_START
        { 0,0 },          // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+1
        { 0,0 },          // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+2
        { 0,0 },   // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+3
        { 0,0 },   // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+4
        { 0,0 },    // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+5
        { 0,0 },    // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+6
            // MOBILITY values
        { 0,0 },    // MOBILITY_START
        { 0,0 },    // MOBILITY_START+1
        { 5,4 },    // MOBILITY_START+2
        { 3,3 },    // MOBILITY_START+3
            // ROOK_BEHIND_FREE_PAWN values
        { 1,6 },    // ROOK_BEHIND_FREE_PAWN_START
        { -30,5 },          // ROOK_BEHIND_FREE_PAWN_START+1
        { -32,4 },          // ROOK_BEHIND_FREE_PAWN_START+2
        { -14,8 },          // ROOK_BEHIND_FREE_PAWN_START+3
        { -4,12 },          // ROOK_BEHIND_FREE_PAWN_START+4
        { 10,-4 },          // ROOK_BEHIND_FREE_PAWN_START+5
        { -4,-31 },         // ROOK_BEHIND_FREE_PAWN_START+6
        { 0,0 },    // ROOK_BEHIND_FREE_PAWN_START+7
        { 30,-1 },          // ROOK_ON_OPEN_FILE,
        { 26,-18 },         // ROOK_ON_SEMI_OPEN_FILE,
        { 2,0 },    // CONNECTED_ROOKS,
        { 23,64 },          // BISHOP_PAIR,
        { -4,-16 },         // BAD_BISHOP_BLOCKED,
        { 0,-2 },   // BAD_BISHOP_UNBLOCKED,
        { -103,-93 },       // TRAPPED_BISHOP,
        { -59,-99 },        // TRAPPED_KNIGHT,
        { 4,3 },    // FIANCHETTO_BISHOP,
        { -4,-3 },          // BROKEN_FIANCHETTO,
        { 52,3 },   // BISHOP_OUTPOST_NO_OPPOSITE_BISHOP,
        { 32,16 },          // BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP,
        { 40,15 },          // KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP,
        { 31,18 },          // KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP,
};

