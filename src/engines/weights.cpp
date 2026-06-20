#include "evaluation.h"
#include "pst.h"

EvaluationResult EvalWeights[PARAM_COUNT1] = {
        {100, 100},        // PAWN
        { 340,255 },        // KNIGHT
        { 280,242 },        // BISHOP
        { 334,416 },        // ROOK
        { 782,889 },       // QUEEN

        // Positional weights
        // PAWN Values
        { MG_PST[0][0], EG_PST[0][0] },    // PAWN_PST_START
        { MG_PST[0][1], EG_PST[0][1] },    // PAWN_PST_START+1
        { MG_PST[0][2], EG_PST[0][2] },    // PAWN_PST_START+2
        { MG_PST[0][3], EG_PST[0][3] },    // PAWN_PST_START+3
        { MG_PST[0][4], EG_PST[0][4] },    // PAWN_PST_START+4
        { MG_PST[0][5], EG_PST[0][5] },    // PAWN_PST_START+5
        { MG_PST[0][6], EG_PST[0][6] },    // PAWN_PST_START+6
        { MG_PST[0][7], EG_PST[0][7] },    // PAWN_PST_START+7
        { MG_PST[0][8], EG_PST[0][8] },    // PAWN_PST_START+8
        { MG_PST[0][9], EG_PST[0][9] },    // PAWN_PST_START+9
        { MG_PST[0][10], EG_PST[0][10] },  // PAWN_PST_START+10
        { MG_PST[0][11], EG_PST[0][11] },  // PAWN_PST_START+11
        { MG_PST[0][12], EG_PST[0][12] },  // PAWN_PST_START+12
        { MG_PST[0][13], EG_PST[0][13] },  // PAWN_PST_START+13
        { MG_PST[0][14], EG_PST[0][14] },  // PAWN_PST_START+14
        { MG_PST[0][15], EG_PST[0][15] },  // PAWN_PST_START+15
        { MG_PST[0][16], EG_PST[0][16] },  // PAWN_PST_START+16
        { MG_PST[0][17], EG_PST[0][17] },  // PAWN_PST_START+17
        { MG_PST[0][18], EG_PST[0][18] },  // PAWN_PST_START+18
        { MG_PST[0][19], EG_PST[0][19] },  // PAWN_PST_START+19
        { MG_PST[0][20], EG_PST[0][20] },  // PAWN_PST_START+20
        { MG_PST[0][21], EG_PST[0][21] },  // PAWN_PST_START+21
        { MG_PST[0][22], EG_PST[0][22] },  // PAWN_PST_START+22
        { MG_PST[0][23], EG_PST[0][23] },  // PAWN_PST_START+23
        { MG_PST[0][24], EG_PST[0][24] },  // PAWN_PST_START+24
        { MG_PST[0][25], EG_PST[0][25] },  // PAWN_PST_START+25
        { MG_PST[0][26], EG_PST[0][26] },  // PAWN_PST_START+26
        { MG_PST[0][27], EG_PST[0][27] },  // PAWN_PST_START+27
        { MG_PST[0][28], EG_PST[0][28] },  // PAWN_PST_START+28
        { MG_PST[0][29], EG_PST[0][29] },  // PAWN_PST_START+29
        { MG_PST[0][30], EG_PST[0][30] },  // PAWN_PST_START+30
        { MG_PST[0][31], EG_PST[0][31] },  // PAWN_PST_START+31
        { MG_PST[0][32], EG_PST[0][32] },  // PAWN_PST_START+32
        { MG_PST[0][33], EG_PST[0][33] },  // PAWN_PST_START+33
        { MG_PST[0][34], EG_PST[0][34] },  // PAWN_PST_START+34
        { MG_PST[0][35], EG_PST[0][35] },  // PAWN_PST_START+35
        { MG_PST[0][36], EG_PST[0][36] },  // PAWN_PST_START+36
        { MG_PST[0][37], EG_PST[0][37] },  // PAWN_PST_START+37
        { MG_PST[0][38], EG_PST[0][38] },  // PAWN_PST_START+38
        { MG_PST[0][39], EG_PST[0][39] },  // PAWN_PST_START+39
        { MG_PST[0][40], EG_PST[0][40] },  // PAWN_PST_START+40
        { MG_PST[0][41], EG_PST[0][41] },  // PAWN_PST_START+41
        { MG_PST[0][42], EG_PST[0][42] },  // PAWN_PST_START+42
        { MG_PST[0][43], EG_PST[0][43] },  // PAWN_PST_START+43
        { MG_PST[0][44], EG_PST[0][44] },  // PAWN_PST_START+44
        { MG_PST[0][45], EG_PST[0][45] },  // PAWN_PST_START+45
        { MG_PST[0][46], EG_PST[0][46] },  // PAWN_PST_START+46
        { MG_PST[0][47], EG_PST[0][47] },  // PAWN_PST_START+47
        { MG_PST[0][48], EG_PST[0][48] },  // PAWN_PST_START+48
        { MG_PST[0][49], EG_PST[0][49] },  // PAWN_PST_START+49
        { MG_PST[0][50], EG_PST[0][50] },  // PAWN_PST_START+50
        { MG_PST[0][51], EG_PST[0][51] },  // PAWN_PST_START+51
        { MG_PST[0][52], EG_PST[0][52] },  // PAWN_PST_START+52
        { MG_PST[0][53], EG_PST[0][53] },  // PAWN_PST_START+53
        { MG_PST[0][54], EG_PST[0][54] },  // PAWN_PST_START+54
        { MG_PST[0][55], EG_PST[0][55] },  // PAWN_PST_START+55
        { MG_PST[0][56], EG_PST[0][56] },  // PAWN_PST_START+56
        { MG_PST[0][57], EG_PST[0][57] },  // PAWN_PST_START+57
        { MG_PST[0][58], EG_PST[0][58] },  // PAWN_PST_START+58
        { MG_PST[0][59], EG_PST[0][59] },  // PAWN_PST_START+59
        { MG_PST[0][60], EG_PST[0][60] },  // PAWN_PST_START+60
        { MG_PST[0][61], EG_PST[0][61] },  // PAWN_PST_START+61
        { MG_PST[0][62], EG_PST[0][62] },  // PAWN_PST_START+62
        { MG_PST[0][63], EG_PST[0][63] },  // PAWN_PST_START+63

        // KNIGHT Values
        { MG_PST[1][0], EG_PST[1][0] },    // KNIGHT_PST_START
        { MG_PST[1][1], EG_PST[1][1] },    // KNIGHT_PST_START+1
        { MG_PST[1][2], EG_PST[1][2] },    // KNIGHT_PST_START+2
        { MG_PST[1][3], EG_PST[1][3] },    // KNIGHT_PST_START+3
        { MG_PST[1][4], EG_PST[1][4] },    // KNIGHT_PST_START+4
        { MG_PST[1][5], EG_PST[1][5] },    // KNIGHT_PST_START+5
        { MG_PST[1][6], EG_PST[1][6] },    // KNIGHT_PST_START+6
        { MG_PST[1][7], EG_PST[1][7] },    // KNIGHT_PST_START+7
        { MG_PST[1][8], EG_PST[1][8] },    // KNIGHT_PST_START+8
        { MG_PST[1][9], EG_PST[1][9] },    // KNIGHT_PST_START+9
        { MG_PST[1][10], EG_PST[1][10] },  // KNIGHT_PST_START+10
        { MG_PST[1][11], EG_PST[1][11] },  // KNIGHT_PST_START+11
        { MG_PST[1][12], EG_PST[1][12] },  // KNIGHT_PST_START+12
        { MG_PST[1][13], EG_PST[1][13] },  // KNIGHT_PST_START+13
        { MG_PST[1][14], EG_PST[1][14] },  // KNIGHT_PST_START+14
        { MG_PST[1][15], EG_PST[1][15] },  // KNIGHT_PST_START+15
        { MG_PST[1][16], EG_PST[1][16] },  // KNIGHT_PST_START+16
        { MG_PST[1][17], EG_PST[1][17] },  // KNIGHT_PST_START+17
        { MG_PST[1][18], EG_PST[1][18] },  // KNIGHT_PST_START+18
        { MG_PST[1][19], EG_PST[1][19] },  // KNIGHT_PST_START+19
        { MG_PST[1][20], EG_PST[1][20] },  // KNIGHT_PST_START+20
        { MG_PST[1][21], EG_PST[1][21] },  // KNIGHT_PST_START+21
        { MG_PST[1][22], EG_PST[1][22] },  // KNIGHT_PST_START+22
        { MG_PST[1][23], EG_PST[1][23] },  // KNIGHT_PST_START+23
        { MG_PST[1][24], EG_PST[1][24] },  // KNIGHT_PST_START+24
        { MG_PST[1][25], EG_PST[1][25] },  // KNIGHT_PST_START+25
        { MG_PST[1][26], EG_PST[1][26] },  // KNIGHT_PST_START+26
        { MG_PST[1][27], EG_PST[1][27] },  // KNIGHT_PST_START+27
        { MG_PST[1][28], EG_PST[1][28] },  // KNIGHT_PST_START+28
        { MG_PST[1][29], EG_PST[1][29] },  // KNIGHT_PST_START+29
        { MG_PST[1][30], EG_PST[1][30] },  // KNIGHT_PST_START+30
        { MG_PST[1][31], EG_PST[1][31] },  // KNIGHT_PST_START+31
        { MG_PST[1][32], EG_PST[1][32] },  // KNIGHT_PST_START+32
        { MG_PST[1][33], EG_PST[1][33] },  // KNIGHT_PST_START+33
        { MG_PST[1][34], EG_PST[1][34] },  // KNIGHT_PST_START+34
        { MG_PST[1][35], EG_PST[1][35] },  // KNIGHT_PST_START+35
        { MG_PST[1][36], EG_PST[1][36] },  // KNIGHT_PST_START+36
        { MG_PST[1][37], EG_PST[1][37] },  // KNIGHT_PST_START+37
        { MG_PST[1][38], EG_PST[1][38] },  // KNIGHT_PST_START+38
        { MG_PST[1][39], EG_PST[1][39] },  // KNIGHT_PST_START+39
        { MG_PST[1][40], EG_PST[1][40] },  // KNIGHT_PST_START+40
        { MG_PST[1][41], EG_PST[1][41] },  // KNIGHT_PST_START+41
        { MG_PST[1][42], EG_PST[1][42] },  // KNIGHT_PST_START+42
        { MG_PST[1][43], EG_PST[1][43] },  // KNIGHT_PST_START+43
        { MG_PST[1][44], EG_PST[1][44] },  // KNIGHT_PST_START+44
        { MG_PST[1][45], EG_PST[1][45] },  // KNIGHT_PST_START+45
        { MG_PST[1][46], EG_PST[1][46] },  // KNIGHT_PST_START+46
        { MG_PST[1][47], EG_PST[1][47] },  // KNIGHT_PST_START+47
        { MG_PST[1][48], EG_PST[1][48] },  // KNIGHT_PST_START+48
        { MG_PST[1][49], EG_PST[1][49] },  // KNIGHT_PST_START+49
        { MG_PST[1][50], EG_PST[1][50] },  // KNIGHT_PST_START+50
        { MG_PST[1][51], EG_PST[1][51] },  // KNIGHT_PST_START+51
        { MG_PST[1][52], EG_PST[1][52] },  // KNIGHT_PST_START+52
        { MG_PST[1][53], EG_PST[1][53] },  // KNIGHT_PST_START+53
        { MG_PST[1][54], EG_PST[1][54] },  // KNIGHT_PST_START+54
        { MG_PST[1][55], EG_PST[1][55] },  // KNIGHT_PST_START+55
        { MG_PST[1][56], EG_PST[1][56] },  // KNIGHT_PST_START+56
        { MG_PST[1][57], EG_PST[1][57] },  // KNIGHT_PST_START+57
        { MG_PST[1][58], EG_PST[1][58] },  // KNIGHT_PST_START+58
        { MG_PST[1][59], EG_PST[1][59] },  // KNIGHT_PST_START+59
        { MG_PST[1][60], EG_PST[1][60] },  // KNIGHT_PST_START+60
        { MG_PST[1][61], EG_PST[1][61] },  // KNIGHT_PST_START+61
        { MG_PST[1][62], EG_PST[1][62] },  // KNIGHT_PST_START+62
        { MG_PST[1][63], EG_PST[1][63] },  // KNIGHT_PST_START+63

        // BISHOP Values
        { MG_PST[2][0], EG_PST[2][0] },    // BISHOP_PST_START
        { MG_PST[2][1], EG_PST[2][1] },    // BISHOP_PST_START+1
        { MG_PST[2][2], EG_PST[2][2] },    // BISHOP_PST_START+2
        { MG_PST[2][3], EG_PST[2][3] },    // BISHOP_PST_START+3
        { MG_PST[2][4], EG_PST[2][4] },    // BISHOP_PST_START+4
        { MG_PST[2][5], EG_PST[2][5] },    // BISHOP_PST_START+5
        { MG_PST[2][6], EG_PST[2][6] },    // BISHOP_PST_START+6
        { MG_PST[2][7], EG_PST[2][7] },    // BISHOP_PST_START+7
        { MG_PST[2][8], EG_PST[2][8] },    // BISHOP_PST_START+8
        { MG_PST[2][9], EG_PST[2][9] },    // BISHOP_PST_START+9
        { MG_PST[2][10], EG_PST[2][10] },  // BISHOP_PST_START+10
        { MG_PST[2][11], EG_PST[2][11] },  // BISHOP_PST_START+11
        { MG_PST[2][12], EG_PST[2][12] },  // BISHOP_PST_START+12
        { MG_PST[2][13], EG_PST[2][13] },  // BISHOP_PST_START+13
        { MG_PST[2][14], EG_PST[2][14] },  // BISHOP_PST_START+14
        { MG_PST[2][15], EG_PST[2][15] },  // BISHOP_PST_START+15
        { MG_PST[2][16], EG_PST[2][16] },  // BISHOP_PST_START+16
        { MG_PST[2][17], EG_PST[2][17] },  // BISHOP_PST_START+17
        { MG_PST[2][18], EG_PST[2][18] },  // BISHOP_PST_START+18
        { MG_PST[2][19], EG_PST[2][19] },  // BISHOP_PST_START+19
        { MG_PST[2][20], EG_PST[2][20] },  // BISHOP_PST_START+20
        { MG_PST[2][21], EG_PST[2][21] },  // BISHOP_PST_START+21
        { MG_PST[2][22], EG_PST[2][22] },  // BISHOP_PST_START+22
        { MG_PST[2][23], EG_PST[2][23] },  // BISHOP_PST_START+23
        { MG_PST[2][24], EG_PST[2][24] },  // BISHOP_PST_START+24
        { MG_PST[2][25], EG_PST[2][25] },  // BISHOP_PST_START+25
        { MG_PST[2][26], EG_PST[2][26] },  // BISHOP_PST_START+26
        { MG_PST[2][27], EG_PST[2][27] },  // BISHOP_PST_START+27
        { MG_PST[2][28], EG_PST[2][28] },  // BISHOP_PST_START+28
        { MG_PST[2][29], EG_PST[2][29] },  // BISHOP_PST_START+29
        { MG_PST[2][30], EG_PST[2][30] },  // BISHOP_PST_START+30
        { MG_PST[2][31], EG_PST[2][31] },  // BISHOP_PST_START+31
        { MG_PST[2][32], EG_PST[2][32] },  // BISHOP_PST_START+32
        { MG_PST[2][33], EG_PST[2][33] },  // BISHOP_PST_START+33
        { MG_PST[2][34], EG_PST[2][34] },  // BISHOP_PST_START+34
        { MG_PST[2][35], EG_PST[2][35] },  // BISHOP_PST_START+35
        { MG_PST[2][36], EG_PST[2][36] },  // BISHOP_PST_START+36
        { MG_PST[2][37], EG_PST[2][37] },  // BISHOP_PST_START+37
        { MG_PST[2][38], EG_PST[2][38] },  // BISHOP_PST_START+38
        { MG_PST[2][39], EG_PST[2][39] },  // BISHOP_PST_START+39
        { MG_PST[2][40], EG_PST[2][40] },  // BISHOP_PST_START+40
        { MG_PST[2][41], EG_PST[2][41] },  // BISHOP_PST_START+41
        { MG_PST[2][42], EG_PST[2][42] },  // BISHOP_PST_START+42
        { MG_PST[2][43], EG_PST[2][43] },  // BISHOP_PST_START+43
        { MG_PST[2][44], EG_PST[2][44] },  // BISHOP_PST_START+44
        { MG_PST[2][45], EG_PST[2][45] },  // BISHOP_PST_START+45
        { MG_PST[2][46], EG_PST[2][46] },  // BISHOP_PST_START+46
        { MG_PST[2][47], EG_PST[2][47] },  // BISHOP_PST_START+47
        { MG_PST[2][48], EG_PST[2][48] },  // BISHOP_PST_START+48
        { MG_PST[2][49], EG_PST[2][49] },  // BISHOP_PST_START+49
        { MG_PST[2][50], EG_PST[2][50] },  // BISHOP_PST_START+50
        { MG_PST[2][51], EG_PST[2][51] },  // BISHOP_PST_START+51
        { MG_PST[2][52], EG_PST[2][52] },  // BISHOP_PST_START+52
        { MG_PST[2][53], EG_PST[2][53] },  // BISHOP_PST_START+53
        { MG_PST[2][54], EG_PST[2][54] },  // BISHOP_PST_START+54
        { MG_PST[2][55], EG_PST[2][55] },  // BISHOP_PST_START+55
        { MG_PST[2][56], EG_PST[2][56] },  // BISHOP_PST_START+56
        { MG_PST[2][57], EG_PST[2][57] },  // BISHOP_PST_START+57
        { MG_PST[2][58], EG_PST[2][58] },  // BISHOP_PST_START+58
        { MG_PST[2][59], EG_PST[2][59] },  // BISHOP_PST_START+59
        { MG_PST[2][60], EG_PST[2][60] },  // BISHOP_PST_START+60
        { MG_PST[2][61], EG_PST[2][61] },  // BISHOP_PST_START+61
        { MG_PST[2][62], EG_PST[2][62] },  // BISHOP_PST_START+62
        { MG_PST[2][63], EG_PST[2][63] },  // BISHOP_PST_START+63

        // ROOK Values
        { MG_PST[3][0], EG_PST[3][0] },    // ROOK_PST_START
        { MG_PST[3][1], EG_PST[3][1] },    // ROOK_PST_START+1
        { MG_PST[3][2], EG_PST[3][2] },    // ROOK_PST_START+2
        { MG_PST[3][3], EG_PST[3][3] },    // ROOK_PST_START+3
        { MG_PST[3][4], EG_PST[3][4] },    // ROOK_PST_START+4
        { MG_PST[3][5], EG_PST[3][5] },    // ROOK_PST_START+5
        { MG_PST[3][6], EG_PST[3][6] },    // ROOK_PST_START+6
        { MG_PST[3][7], EG_PST[3][7] },    // ROOK_PST_START+7
        { MG_PST[3][8], EG_PST[3][8] },    // ROOK_PST_START+8
        { MG_PST[3][9], EG_PST[3][9] },    // ROOK_PST_START+9
        { MG_PST[3][10], EG_PST[3][10] },  // ROOK_PST_START+10
        { MG_PST[3][11], EG_PST[3][11] },  // ROOK_PST_START+11
        { MG_PST[3][12], EG_PST[3][12] },  // ROOK_PST_START+12
        { MG_PST[3][13], EG_PST[3][13] },  // ROOK_PST_START+13
        { MG_PST[3][14], EG_PST[3][14] },  // ROOK_PST_START+14
        { MG_PST[3][15], EG_PST[3][15] },  // ROOK_PST_START+15
        { MG_PST[3][16], EG_PST[3][16] },  // ROOK_PST_START+16
        { MG_PST[3][17], EG_PST[3][17] },  // ROOK_PST_START+17
        { MG_PST[3][18], EG_PST[3][18] },  // ROOK_PST_START+18
        { MG_PST[3][19], EG_PST[3][19] },  // ROOK_PST_START+19
        { MG_PST[3][20], EG_PST[3][20] },  // ROOK_PST_START+20
        { MG_PST[3][21], EG_PST[3][21] },  // ROOK_PST_START+21
        { MG_PST[3][22], EG_PST[3][22] },  // ROOK_PST_START+22
        { MG_PST[3][23], EG_PST[3][23] },  // ROOK_PST_START+23
        { MG_PST[3][24], EG_PST[3][24] },  // ROOK_PST_START+24
        { MG_PST[3][25], EG_PST[3][25] },  // ROOK_PST_START+25
        { MG_PST[3][26], EG_PST[3][26] },  // ROOK_PST_START+26
        { MG_PST[3][27], EG_PST[3][27] },  // ROOK_PST_START+27
        { MG_PST[3][28], EG_PST[3][28] },  // ROOK_PST_START+28
        { MG_PST[3][29], EG_PST[3][29] },  // ROOK_PST_START+29
        { MG_PST[3][30], EG_PST[3][30] },  // ROOK_PST_START+30
        { MG_PST[3][31], EG_PST[3][31] },  // ROOK_PST_START+31
        { MG_PST[3][32], EG_PST[3][32] },  // ROOK_PST_START+32
        { MG_PST[3][33], EG_PST[3][33] },  // ROOK_PST_START+33
        { MG_PST[3][34], EG_PST[3][34] },  // ROOK_PST_START+34
        { MG_PST[3][35], EG_PST[3][35] },  // ROOK_PST_START+35
        { MG_PST[3][36], EG_PST[3][36] },  // ROOK_PST_START+36
        { MG_PST[3][37], EG_PST[3][37] },  // ROOK_PST_START+37
        { MG_PST[3][38], EG_PST[3][38] },  // ROOK_PST_START+38
        { MG_PST[3][39], EG_PST[3][39] },  // ROOK_PST_START+39
        { MG_PST[3][40], EG_PST[3][40] },  // ROOK_PST_START+40
        { MG_PST[3][41], EG_PST[3][41] },  // ROOK_PST_START+41
        { MG_PST[3][42], EG_PST[3][42] },  // ROOK_PST_START+42
        { MG_PST[3][43], EG_PST[3][43] },  // ROOK_PST_START+43
        { MG_PST[3][44], EG_PST[3][44] },  // ROOK_PST_START+44
        { MG_PST[3][45], EG_PST[3][45] },  // ROOK_PST_START+45
        { MG_PST[3][46], EG_PST[3][46] },  // ROOK_PST_START+46
        { MG_PST[3][47], EG_PST[3][47] },  // ROOK_PST_START+47
        { MG_PST[3][48], EG_PST[3][48] },  // ROOK_PST_START+48
        { MG_PST[3][49], EG_PST[3][49] },  // ROOK_PST_START+49
        { MG_PST[3][50], EG_PST[3][50] },  // ROOK_PST_START+50
        { MG_PST[3][51], EG_PST[3][51] },  // ROOK_PST_START+51
        { MG_PST[3][52], EG_PST[3][52] },  // ROOK_PST_START+52
        { MG_PST[3][53], EG_PST[3][53] },  // ROOK_PST_START+53
        { MG_PST[3][54], EG_PST[3][54] },  // ROOK_PST_START+54
        { MG_PST[3][55], EG_PST[3][55] },  // ROOK_PST_START+55
        { MG_PST[3][56], EG_PST[3][56] },  // ROOK_PST_START+56
        { MG_PST[3][57], EG_PST[3][57] },  // ROOK_PST_START+57
        { MG_PST[3][58], EG_PST[3][58] },  // ROOK_PST_START+58
        { MG_PST[3][59], EG_PST[3][59] },  // ROOK_PST_START+59
        { MG_PST[3][60], EG_PST[3][60] },  // ROOK_PST_START+60
        { MG_PST[3][61], EG_PST[3][61] },  // ROOK_PST_START+61
        { MG_PST[3][62], EG_PST[3][62] },  // ROOK_PST_START+62
        { MG_PST[3][63], EG_PST[3][63] },  // ROOK_PST_START+63

        // QUEEN Values
        { MG_PST[4][0], EG_PST[4][0] },    // QUEEN_PST_START
        { MG_PST[4][1], EG_PST[4][1] },    // QUEEN_PST_START+1
        { MG_PST[4][2], EG_PST[4][2] },    // QUEEN_PST_START+2
        { MG_PST[4][3], EG_PST[4][3] },    // QUEEN_PST_START+3
        { MG_PST[4][4], EG_PST[4][4] },    // QUEEN_PST_START+4
        { MG_PST[4][5], EG_PST[4][5] },    // QUEEN_PST_START+5
        { MG_PST[4][6], EG_PST[4][6] },    // QUEEN_PST_START+6
        { MG_PST[4][7], EG_PST[4][7] },    // QUEEN_PST_START+7
        { MG_PST[4][8], EG_PST[4][8] },    // QUEEN_PST_START+8
        { MG_PST[4][9], EG_PST[4][9] },    // QUEEN_PST_START+9
        { MG_PST[4][10], EG_PST[4][10] },  // QUEEN_PST_START+10
        { MG_PST[4][11], EG_PST[4][11] },  // QUEEN_PST_START+11
        { MG_PST[4][12], EG_PST[4][12] },  // QUEEN_PST_START+12
        { MG_PST[4][13], EG_PST[4][13] },  // QUEEN_PST_START+13
        { MG_PST[4][14], EG_PST[4][14] },  // QUEEN_PST_START+14
        { MG_PST[4][15], EG_PST[4][15] },  // QUEEN_PST_START+15
        { MG_PST[4][16], EG_PST[4][16] },  // QUEEN_PST_START+16
        { MG_PST[4][17], EG_PST[4][17] },  // QUEEN_PST_START+17
        { MG_PST[4][18], EG_PST[4][18] },  // QUEEN_PST_START+18
        { MG_PST[4][19], EG_PST[4][19] },  // QUEEN_PST_START+19
        { MG_PST[4][20], EG_PST[4][20] },  // QUEEN_PST_START+20
        { MG_PST[4][21], EG_PST[4][21] },  // QUEEN_PST_START+21
        { MG_PST[4][22], EG_PST[4][22] },  // QUEEN_PST_START+22
        { MG_PST[4][23], EG_PST[4][23] },  // QUEEN_PST_START+23
        { MG_PST[4][24], EG_PST[4][24] },  // QUEEN_PST_START+24
        { MG_PST[4][25], EG_PST[4][25] },  // QUEEN_PST_START+25
        { MG_PST[4][26], EG_PST[4][26] },  // QUEEN_PST_START+26
        { MG_PST[4][27], EG_PST[4][27] },  // QUEEN_PST_START+27
        { MG_PST[4][28], EG_PST[4][28] },  // QUEEN_PST_START+28
        { MG_PST[4][29], EG_PST[4][29] },  // QUEEN_PST_START+29
        { MG_PST[4][30], EG_PST[4][30] },  // QUEEN_PST_START+30
        { MG_PST[4][31], EG_PST[4][31] },  // QUEEN_PST_START+31
        { MG_PST[4][32], EG_PST[4][32] },  // QUEEN_PST_START+32
        { MG_PST[4][33], EG_PST[4][33] },  // QUEEN_PST_START+33
        { MG_PST[4][34], EG_PST[4][34] },  // QUEEN_PST_START+34
        { MG_PST[4][35], EG_PST[4][35] },  // QUEEN_PST_START+35
        { MG_PST[4][36], EG_PST[4][36] },  // QUEEN_PST_START+36
        { MG_PST[4][37], EG_PST[4][37] },  // QUEEN_PST_START+37
        { MG_PST[4][38], EG_PST[4][38] },  // QUEEN_PST_START+38
        { MG_PST[4][39], EG_PST[4][39] },  // QUEEN_PST_START+39
        { MG_PST[4][40], EG_PST[4][40] },  // QUEEN_PST_START+40
        { MG_PST[4][41], EG_PST[4][41] },  // QUEEN_PST_START+41
        { MG_PST[4][42], EG_PST[4][42] },  // QUEEN_PST_START+42
        { MG_PST[4][43], EG_PST[4][43] },  // QUEEN_PST_START+43
        { MG_PST[4][44], EG_PST[4][44] },  // QUEEN_PST_START+44
        { MG_PST[4][45], EG_PST[4][45] },  // QUEEN_PST_START+45
        { MG_PST[4][46], EG_PST[4][46] },  // QUEEN_PST_START+46
        { MG_PST[4][47], EG_PST[4][47] },  // QUEEN_PST_START+47
        { MG_PST[4][48], EG_PST[4][48] },  // QUEEN_PST_START+48
        { MG_PST[4][49], EG_PST[4][49] },  // QUEEN_PST_START+49
        { MG_PST[4][50], EG_PST[4][50] },  // QUEEN_PST_START+50
        { MG_PST[4][51], EG_PST[4][51] },  // QUEEN_PST_START+51
        { MG_PST[4][52], EG_PST[4][52] },  // QUEEN_PST_START+52
        { MG_PST[4][53], EG_PST[4][53] },  // QUEEN_PST_START+53
        { MG_PST[4][54], EG_PST[4][54] },  // QUEEN_PST_START+54
        { MG_PST[4][55], EG_PST[4][55] },  // QUEEN_PST_START+55
        { MG_PST[4][56], EG_PST[4][56] },  // QUEEN_PST_START+56
        { MG_PST[4][57], EG_PST[4][57] },  // QUEEN_PST_START+57
        { MG_PST[4][58], EG_PST[4][58] },  // QUEEN_PST_START+58
        { MG_PST[4][59], EG_PST[4][59] },  // QUEEN_PST_START+59
        { MG_PST[4][60], EG_PST[4][60] },  // QUEEN_PST_START+60
        { MG_PST[4][61], EG_PST[4][61] },  // QUEEN_PST_START+61
        { MG_PST[4][62], EG_PST[4][62] },  // QUEEN_PST_START+62
        { MG_PST[4][63], EG_PST[4][63] },  // QUEEN_PST_START+63

        // KING Values
        { MG_PST[5][0], EG_PST[5][0] },    // KING_PST_START
        { MG_PST[5][1], EG_PST[5][1] },    // KING_PST_START+1
        { MG_PST[5][2], EG_PST[5][2] },    // KING_PST_START+2
        { MG_PST[5][3], EG_PST[5][3] },    // KING_PST_START+3
        { MG_PST[5][4], EG_PST[5][4] },    // KING_PST_START+4
        { MG_PST[5][5], EG_PST[5][5] },    // KING_PST_START+5
        { MG_PST[5][6], EG_PST[5][6] },    // KING_PST_START+6
        { MG_PST[5][7], EG_PST[5][7] },    // KING_PST_START+7
        { MG_PST[5][8], EG_PST[5][8] },    // KING_PST_START+8
        { MG_PST[5][9], EG_PST[5][9] },    // KING_PST_START+9
        { MG_PST[5][10], EG_PST[5][10] },  // KING_PST_START+10
        { MG_PST[5][11], EG_PST[5][11] },  // KING_PST_START+11
        { MG_PST[5][12], EG_PST[5][12] },  // KING_PST_START+12
        { MG_PST[5][13], EG_PST[5][13] },  // KING_PST_START+13
        { MG_PST[5][14], EG_PST[5][14] },  // KING_PST_START+14
        { MG_PST[5][15], EG_PST[5][15] },  // KING_PST_START+15
        { MG_PST[5][16], EG_PST[5][16] },  // KING_PST_START+16
        { MG_PST[5][17], EG_PST[5][17] },  // KING_PST_START+17
        { MG_PST[5][18], EG_PST[5][18] },  // KING_PST_START+18
        { MG_PST[5][19], EG_PST[5][19] },  // KING_PST_START+19
        { MG_PST[5][20], EG_PST[5][20] },  // KING_PST_START+20
        { MG_PST[5][21], EG_PST[5][21] },  // KING_PST_START+21
        { MG_PST[5][22], EG_PST[5][22] },  // KING_PST_START+22
        { MG_PST[5][23], EG_PST[5][23] },  // KING_PST_START+23
        { MG_PST[5][24], EG_PST[5][24] },  // KING_PST_START+24
        { MG_PST[5][25], EG_PST[5][25] },  // KING_PST_START+25
        { MG_PST[5][26], EG_PST[5][26] },  // KING_PST_START+26
        { MG_PST[5][27], EG_PST[5][27] },  // KING_PST_START+27
        { MG_PST[5][28], EG_PST[5][28] },  // KING_PST_START+28
        { MG_PST[5][29], EG_PST[5][29] },  // KING_PST_START+29
        { MG_PST[5][30], EG_PST[5][30] },  // KING_PST_START+30
        { MG_PST[5][31], EG_PST[5][31] },  // KING_PST_START+31
        { MG_PST[5][32], EG_PST[5][32] },  // KING_PST_START+32
        { MG_PST[5][33], EG_PST[5][33] },  // KING_PST_START+33
        { MG_PST[5][34], EG_PST[5][34] },  // KING_PST_START+34
        { MG_PST[5][35], EG_PST[5][35] },  // KING_PST_START+35
        { MG_PST[5][36], EG_PST[5][36] },  // KING_PST_START+36
        { MG_PST[5][37], EG_PST[5][37] },  // KING_PST_START+37
        { MG_PST[5][38], EG_PST[5][38] },  // KING_PST_START+38
        { MG_PST[5][39], EG_PST[5][39] },  // KING_PST_START+39
        { MG_PST[5][40], EG_PST[5][40] },  // KING_PST_START+40
        { MG_PST[5][41], EG_PST[5][41] },  // KING_PST_START+41
        { MG_PST[5][42], EG_PST[5][42] },  // KING_PST_START+42
        { MG_PST[5][43], EG_PST[5][43] },  // KING_PST_START+43
        { MG_PST[5][44], EG_PST[5][44] },  // KING_PST_START+44
        { MG_PST[5][45], EG_PST[5][45] },  // KING_PST_START+45
        { MG_PST[5][46], EG_PST[5][46] },  // KING_PST_START+46
        { MG_PST[5][47], EG_PST[5][47] },  // KING_PST_START+47
        { MG_PST[5][48], EG_PST[5][48] },  // KING_PST_START+48
        { MG_PST[5][49], EG_PST[5][49] },  // KING_PST_START+49
        { MG_PST[5][50], EG_PST[5][50] },  // KING_PST_START+50
        { MG_PST[5][51], EG_PST[5][51] },  // KING_PST_START+51
        { MG_PST[5][52], EG_PST[5][52] },  // KING_PST_START+52
        { MG_PST[5][53], EG_PST[5][53] },  // KING_PST_START+53
        { MG_PST[5][54], EG_PST[5][54] },  // KING_PST_START+54
        { MG_PST[5][55], EG_PST[5][55] },  // KING_PST_START+55
        { MG_PST[5][56], EG_PST[5][56] },  // KING_PST_START+56
        { MG_PST[5][57], EG_PST[5][57] },  // KING_PST_START+57
        { MG_PST[5][58], EG_PST[5][58] },  // KING_PST_START+58
        { MG_PST[5][59], EG_PST[5][59] },  // KING_PST_START+59
        { MG_PST[5][60], EG_PST[5][60] },  // KING_PST_START+60
        { MG_PST[5][61], EG_PST[5][61] },  // KING_PST_START+61
        { MG_PST[5][62], EG_PST[5][62] },  // KING_PST_START+62
        { MG_PST[5][63], EG_PST[5][63] },  // KING_PST_START+63
};
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
        { 21,-13 },         // PAWN_SHIELD_BONUS,
        { -77,-13 },        // DIRECTLY_ON_OPEN_FILE_NEXT_TO_OPEN_PENALTY,
        { -76,-7 },         // DIRECTLY_ON_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY,
        { -33,-17 },        // NEXT_TO_OPEN_FILE_PENALTY,
        { 14,2 },   // DIRECTLY_ON_SEMI_OPEN_FILE_NEXT_TO_OPEN_PENALTY,
        { 8,-17 },          // DIRECTLY_ON_SEMI_OPEN_FILE_NOT_NEXT_TO_OPEN_PENALTY,
        { 4,-11 },          // NEXT_TO_SEMI_OPEN_FILE_PENALTY,
            // NEXT_TO_OPEN_DIAGONAL_PENALTY values
        { -24,-54 },        // NEXT_TO_OPEN_DIAGONAL_PENALTY_START
        { -1,-9 },          // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+1
        { 13,16 },          // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+2
        { 28,4 },   // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+3
        { 12,7 },   // NEXT_TO_OPEN_DIAGONAL_PENALTY_START+4
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

