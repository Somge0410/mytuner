#include "evaluation.h"
#include <memory>
static thread_local std::unique_ptr<std::array<PawnEvalEntry, PAWN_HASH_SIZE>> pawn_evaluation_table;
PawnEvalEntry& get_pawn_entry(size_t idx) {
	if (!pawn_evaluation_table) {
		pawn_evaluation_table = std::make_unique<std::array<PawnEvalEntry, PAWN_HASH_SIZE>>();
	}
	return (*pawn_evaluation_table)[idx];
}
bool trace_eval_agree(const Board& board, const EvaluationResult weights[PARAM_COUNT]) {
	Trace trace;
	int eval = evaluate<true>(board, &trace);
	int trace_eval = get_trace_eval(&trace, weights, board.get_game_phase());
	if (trace_eval != eval) {
		std::cerr << "Trace evaluation" << trace_eval << " does not match actual evaluation " << eval << std::endl;
	}
	return trace_eval == eval;
}
template<bool isTracing>
int evaluate(const Board& board, Trace* trace, uint8_t terms_mask) {
	EvaluationResult score = { 0,0 };
	EvalContext ctx(board);

	eval_material<isTracing>(score, board, trace);
	eval_positional<isTracing>(score, board, trace);
	eval_pawns<isTracing>(score, ctx, trace);
	if (terms_mask != EvalAll) return tapered(score, board.get_game_phase());
	eval_king_safety<isTracing>(score, ctx, trace);

	eval_mobility<isTracing>(score, ctx, trace);
	eval_rook_activity<isTracing>(score, ctx, trace);
	eval_minor_pieces<isTracing>(score, ctx, trace);
	return tapered({0,0}, board.get_game_phase());

}
template int evaluate<false>(const Board& board, Trace* trace, uint8_t terms_mask);
template int evaluate<true>(const Board& board, Trace* trace, uint8_t terms_mask);

template<bool isTracing>
void eval_material(EvaluationResult& score, const Board& board, Trace* trace) {
	score += board.get_material_score();
	if (isTracing && trace) {
		trace->add(EvalParam::PAWN, popcount(board.get_pieces(Color::WHITE, PieceType::PAWN)) - popcount(board.get_pieces(Color::BLACK, PieceType::PAWN)));
		trace->add(EvalParam::KNIGHT, popcount(board.get_pieces(Color::WHITE, PieceType::KNIGHT)) - popcount(board.get_pieces(Color::BLACK, PieceType::KNIGHT)));
		trace->add(EvalParam::BISHOP, popcount(board.get_pieces(Color::WHITE, PieceType::BISHOP)) - popcount(board.get_pieces(Color::BLACK, PieceType::BISHOP)));
		trace->add(EvalParam::ROOK, popcount(board.get_pieces(Color::WHITE, PieceType::ROOK)) - popcount(board.get_pieces(Color::BLACK, PieceType::ROOK)));
		trace->add(EvalParam::QUEEN, popcount(board.get_pieces(Color::WHITE, PieceType::QUEEN)) - popcount(board.get_pieces(Color::BLACK, PieceType::QUEEN)));
	}
}
template <bool isTracing>
void eval_positional(EvaluationResult& score, const Board& board, Trace* trace) {
	score += board.get_positional_score();
	// No tracing for positional score for now
	if (isTracing && trace) {
		for (int pieceType = 0; pieceType < 6; pieceType++) {
			for (int color = 0; color < 2; color++) {
				uint64_t pieces = board.get_pieces(static_cast<Color>(color), static_cast<PieceType>(pieceType));
				while (pieces) {
					int square = poplsb(pieces);
					if (color == 1) square = flip_square(square);
					EvalParam param = static_cast<EvalParam>(pieceType * 64 + square + EvalParam::PAWN_PST_START);
					trace->add(param, color == 0 ? 1 : -1);
				}
			}
		}
	}
}
template <bool isTracing>
void eval_pawns(EvaluationResult& score, EvalContext& ctx, Trace* trace) {

	uint64_t pawn_key = ctx.board.get_pawn_key();
	int idx = pawn_key & (PAWN_HASH_SIZE - 1);
	PawnEvalEntry& entry = get_pawn_entry(idx);
	if (!isTracing && entry.valid && entry.key == pawn_key) {
		score += entry.score;
		ctx.files_with_no_color_pawns[0] = entry.file_info[0];
		ctx.files_with_no_color_pawns[1] = entry.file_info[1];
		return;
	}
	EvaluationResult entry_score = { 0,0 };
	ctx.init_file_info();
	eval_iso_passed<isTracing>(entry_score, ctx, trace);
	eval_backward<isTracing>(entry_score, ctx, trace);
	eval_double_pawns<isTracing>(entry_score, ctx, trace);

	score += entry_score;
	if (!isTracing) {
		entry.key = pawn_key;
		entry.score = entry_score;
		entry.file_info[0] = ctx.files_with_no_color_pawns[0];
		entry.file_info[1] = ctx.files_with_no_color_pawns[1];
		entry.valid = true;
	}
}
template <bool isTracing>
void eval_iso_passed(EvaluationResult& score, EvalContext& ctx, Trace* trace) {
	for (size_t color = 0; color < 2; color++) {
		int ecolor = color == 0 ? 1 : 0;
		uint64_t pawns = ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN);
		while (pawns)
		{
			int pawn_square = get_lsb(pawns);
			int file_index = pawn_square % 8;
			int rank_index = color == rank(pawn_square);
			int bucket = PASSED_PAWN_BUCKET[color == 0 ? pawn_square : flip_square(pawn_square)];
			if ((ctx.board.get_pieces(static_cast<Color>(ecolor), PieceType::PAWN) & PASSED_PAWN_MASK[color][pawn_square]) == 0)
			{
				ctx.passed[color] |= (1ULL << pawn_square);
				if (color == 0) {
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::PASSED_PAWNS_START + bucket), 1, trace);
				}
				else {
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::PASSED_PAWNS_START + bucket), -1, trace);

				}
				//check if passed pawn is defended
				uint64_t defenders = get_pawn_attacks(bit64(pawn_square), static_cast<Color>(ecolor));
				int def_count = popcount(defenders & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN));
				addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::PROTECTED_PASSED_PAWNS_START + bucket), color == 0 ? def_count : -def_count, trace);
				//check if blockated
				int block_count = is_occupied(get_forward_square(pawn_square, static_cast<Color>(color)), ctx.board.get_all_pieces()) ? 1 : 0;
				addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::BLOCKED_FREE_PAWN_START + bucket), color == 0 ? block_count : -block_count, trace);
				// check if enemy king can stop it
				if (block_count == 0) {
					int promo_square = get_promotion_square(pawn_square, static_cast<Color>(color));
					int enemy_king_distance_to_promo_sq = king_distance(ctx.board.get_king_square(static_cast<Color>(ecolor)), promo_square);
					int pawn_distance_to_promo_sq = color == 0 ? 7 - rank_index : rank_index;
					if (ctx.board.get_turn() == static_cast<Color>(ecolor)) enemy_king_distance_to_promo_sq--;
					if (enemy_king_distance_to_promo_sq > pawn_distance_to_promo_sq) {
						addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::CANT_REACHED_BY_ENEMY_KING_START + bucket), color == 0 ? 1 : -1, trace);
					}
				}
				// Check if own king is close
				int own_king_distance_to_pawn = king_distance(ctx.board.get_king_square(static_cast<Color>(color)), pawn_square);
				if (own_king_distance_to_pawn <= 2) {
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::OWN_KING_IS_CLOSE_START + bucket), color == 0 ? 1 : -1, trace);
				}
				if (own_king_distance_to_pawn >= 5) {
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::OWN_KING_IS_FAR_START + bucket), color == 0 ? 1 : -1, trace);
				}
				//Check if Rook is behing pawn
				uint64_t rooks = ctx.board.get_pieces(static_cast<Color>(color), PieceType::ROOK) & FORWARD_WAY_MASK[ecolor][pawn_square];
				while (rooks) {
					int rook_square = get_lsb(rooks);
					if (bit64(pawn_square) & get_rook_attacks(rook_square, ctx.board.get_all_pieces())) {
						addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::ROOK_BEHIND_FREE_PAWN_START + bucket), color == 0 ? 1 : -1, trace);
						break;
					}
					rooks &= rooks - 1;
				}
				//Check if Opponent Rook is behind pawn
				uint64_t op_rooks = ctx.board.get_pieces(static_cast<Color>(ecolor), PieceType::ROOK) & FORWARD_WAY_MASK[ecolor][pawn_square];
				while (op_rooks) {
					int rook_square = get_lsb(op_rooks);
					if (bit64(pawn_square) & get_rook_attacks(rook_square, ctx.board.get_all_pieces())) {
						addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::OP_ROOK_BEHIND_FREE_PAWN_START + bucket), color == 0 ? 1 : -1, trace);
						break;
					}
					op_rooks &= op_rooks - 1;
				}
				pawns &= pawns - 1;
				continue;
			}
			if ((ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN) & ADJACENT_FILE_MASK[file_index]) == 0)
			{
				ctx.isolated[color] |= (1ULL << pawn_square);
				int bucket = ISOLATED_PAWN_BUCKET[color == 0 ? pawn_square : flip_square(pawn_square)];
				if (is_occupied(get_forward_square(pawn_square, static_cast<Color>(color)), ctx.board.get_all_pieces()))
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::BLOCKED_ISOLANI_START + bucket), color == 0 ? 1 : -1, trace);
				else
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::ISOLANI_START + bucket), color == 0 ? 1 : -1, trace);
				uint64_t defends = ctx.get_attacks(static_cast<Color>(color)) & bit64(pawn_square);
				if (defends != 0) {
					addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::PROTECTED_PASSED_PAWNS_START + bucket), color == 0 ? 1 : -1, trace);
				}

			}
			pawns &= pawns - 1;
		}
	}
}
template <bool isTracing>
void eval_backward(EvaluationResult& score, EvalContext& ctx, Trace* trace) {
	int blocked_backward_count = 0;
	int forwad_controlled_backward_count = 0;
	int free_to_advance_backward_count = 0;
	for (size_t color = 0; color < 2; color++) {
		int ecolor = color == 0 ? 1 : 0;
		uint64_t pawns = ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN) & ~ctx.passed[color] & ~ctx.isolated[color];

		while (pawns)
		{
			int pawn_square = get_lsb(pawns);
			int file_index = pawn_square % 8;
			int forward_square = color == to_int(Color::WHITE) ? pawn_square + 8 : pawn_square - 8;

			if (forward_square >= 0 && forward_square < 64) {
				uint64_t forward_mask = bit64(forward_square);
				uint64_t adjacent_backwards = PAWN_ATTACKS[ecolor][pawn_square];
				bool has_adjacent_support = (ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN) & adjacent_backwards) != 0;
				bool forward_blocked = (ctx.board.get_all_pieces() & forward_mask) != 0;
				if (!has_adjacent_support) {
					if (forward_blocked) {
						ctx.backward[color] |= (1ULL << pawn_square);
						blocked_backward_count += color == 0 ? 1 : -1;
					}
					else if ((PAWN_ATTACKS[color][forward_square] & ctx.board.get_pieces(static_cast<Color>(ecolor), PieceType::PAWN)) != 0)
					{
						ctx.backward[color] |= (1ULL << pawn_square);
						forwad_controlled_backward_count += color == 0 ? 1 : -1;

					}
					else {
						free_to_advance_backward_count += color == 0 ? 1 : -1;
					}
				}

			}
			pawns &= pawns - 1;
		}

	}
	addTerm<isTracing>(score, EvalParam::FORWARD_BLOCKED_BACKWARD, blocked_backward_count, trace);
	addTerm<isTracing>(score, EvalParam::FORWARD_CONTROLLED_BACKWARD, forwad_controlled_backward_count, trace);
	addTerm<isTracing>(score, EvalParam::FREE_TO_ADV_BACKWARD, free_to_advance_backward_count, trace);
}
template <bool isTracing>
void eval_double_pawns(EvaluationResult& score, EvalContext& ctx, Trace* trace) {
	int doubled_count = 0;
	for (size_t file = 0; file < 8; ++file)
	{
		doubled_count = 0;
		uint64_t file_mask = FILE_MASK[file];
		int white_doubled = popcount(ctx.board.get_pieces(static_cast<Color>(Color::WHITE), PieceType::PAWN) & file_mask);
		if (white_doubled > 1)
		{
			doubled_count += white_doubled - 1;


		}
		int black_doubled = popcount(ctx.board.get_pieces(static_cast<Color>(Color::BLACK), PieceType::PAWN) & file_mask);
		if (black_doubled > 1)
		{
			doubled_count -= black_doubled - 1;
		}

		addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::DOUBLE_PAWN_FILE_START + file), doubled_count, trace);
	}
}
template<bool isTracing>
void eval_king_safety(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int king_squares[2] = { ctx.board.get_king_square(Color::WHITE), ctx.board.get_king_square(Color::BLACK) };
	int king_file_open_with_rq = 0;
	int king_file_open_without_rq = 0;
	int adj_file_open_with_rq = 0;
	int adj_file_open_without_rq = 0;
	int no_def_pawns_on_king_file_with_rq = 0;
	int no_def_pawns_on_king_file_without_rq = 0;
	int no_att_pawns_on_king_file_with_rq = 0;
	int no_att_pawns_on_king_file_without_rq = 0;
	int files_around_king_without_def_pawns_without_rq = 0;
	int files_around_king_without_def_pawns_with_rq = 0;
	int files_around_king_without_att_pawns_with_rq = 0;
	int files_around_king_without_att_pawns_without_rq = 0;
	int pawns_defend_diagonal[3] = { 0,0,0 };
	int other_def_count_no_pawns[3] = { 0,0,0 };
	int other_def_count_with_pawns[3] = { 0,0,0 };
	int right_def_queen_side_square_with_qb = 0;
	int right_def_queen_side_square_without_qb = 0;
	int left_def_queen_side_square_with_qb = 0;
	int left_def_queen_side_square_without_qb = 0;
	int left_def_central_square_with_qb = 0;
	int left_def_central_square_without_qb = 0;
	int right_def_central_square_with_qb = 0;
	int right_def_central_square_without_qb = 0;
	int center_forward_square = 0;
	int left_def_king_side_square_with_qb = 0;
	int left_def_king_side_square_without_qb = 0;
	int right_def_king_side_square_with_qb = 0;
	int right_def_king_side_square_without_qb = 0;
	int queen_side_forward_square = 0;
	int king_side_forward_square = 0;
	int small_attack_count[8] = { 0 };
	int big_attack_count[13] = { 0 };
	int piece_attacking[6] = { 0 };
	int distinct_pieces_attacking[5] = { 0 };
	int weak_pawns_around_small_king = 0;
	int weak_pawns_around_big_king = 0;
	int op_pawns_in_small_mask = 0;
	int op_pawns_in_big_mask = 0;
	int king_escape_squares[3] = { 0 };
	int king_tropism[7] = { 0 };
	int mobility_penalty = 0;
	for (size_t color = 0; color < 2; color++) {
		int king_file = file(king_squares[color]);
		int op_color = flip_color(color);
		bool has_rq = (ctx.get_pieces(static_cast<Color>(op_color), PieceType::ROOK) | ctx.get_pieces(static_cast<Color>(op_color), PieceType::QUEEN)) != 0;
		if (ctx.is_file_open(king_file)) {
			if (has_rq)
				king_file_open_with_rq += color == 0 ? 1 : -1;
			else
				king_file_open_without_rq += color == 0 ? 1 : -1;
		}
		else {
			if (!ctx.does_color_have_pawns_on_file(king_file, static_cast<Color>(color))) {
				if (has_rq)
					no_def_pawns_on_king_file_with_rq += color == 0 ? 1 : -1;
				else
					no_def_pawns_on_king_file_without_rq += color == 0 ? 1 : -1;
			}
			if (!ctx.does_color_have_pawns_on_file(king_file, static_cast<Color>(op_color))) {
				if (has_rq)
					no_att_pawns_on_king_file_with_rq += color == 0 ? 1 : -1;
				else
					no_att_pawns_on_king_file_without_rq += color == 0 ? 1 : -1;
			}
		}
		for (int k : {1, -1}) {
			int adj_file = king_file + k;
			if (adj_file >= 0 && adj_file < 8) {

				if (ctx.is_file_open(adj_file)) {
					if (has_rq)
						adj_file_open_with_rq += color == 0 ? 1 : -1;
					else
						adj_file_open_without_rq += color == 0 ? 1 : -1;
				}
				else {
					if (!ctx.does_color_have_pawns_on_file(adj_file, static_cast<Color>(color))) {
						if (has_rq)
							files_around_king_without_def_pawns_with_rq += color == 0 ? 1 : -1;
						else
							files_around_king_without_def_pawns_without_rq += color == 0 ? 1 : -1;
					}
					if (!ctx.does_color_have_pawns_on_file(adj_file, static_cast<Color>(op_color))) {
						if (has_rq)
							files_around_king_without_att_pawns_with_rq += color == 0 ? 1 : -1;
						else
							files_around_king_without_att_pawns_without_rq += color == 0 ? 1 : -1;
					}
				}
			}
		}
		// Open Diagonal Penalty
		uint64_t bishop_attack_mask = get_bishop_attacks(king_squares[color], ctx.get_color_pieces(op_color));
		uint64_t op_bishop_queen_on_mask = bishop_attack_mask & (ctx.get_pieces(op_color, PieceType::BISHOP) | ctx.get_pieces(op_color, PieceType::QUEEN));
		while (op_bishop_queen_on_mask) {
			int sq = get_lsb(op_bishop_queen_on_mask);
			uint64_t line_between = LINE_BETWEEN[sq][king_squares[color]];
			int count = popcount(line_between & ctx.get_pieces(color, PieceType::PAWN));
			if (count > 2) count = 2;
			pawns_defend_diagonal[count] += color == 0 ? 1 : -1;
			int other_def_count = popcount(line_between & ctx.get_color_pieces(color) & ~ctx.get_pieces(color, PieceType::PAWN));
			if (other_def_count > 2) other_def_count = 2;
			if (count == 0)
				other_def_count_no_pawns[other_def_count] += color == 0 ? 1 : -1;
			else
				other_def_count_with_pawns[other_def_count] += color == 0 ? 1 : -1;
			op_bishop_queen_on_mask &= op_bishop_queen_on_mask - 1;
		}
		//PAWN_SHIELD
		if (king_file == 0) {
			int forwad_square = get_forward_square(king_squares[color], static_cast<Color>(color));
			if (forwad_square >= 0 && forwad_square < 64) {
				int center_square = forwad_square + 1;
				bool is_light_square = (bit64(center_square) & LIGHT_SQUARES) != 0;
				uint64_t ld_squares = is_light_square ? LIGHT_SQUARES : DARK_SQUARES;
				uint64_t bishops = ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::BISHOP) & ld_squares;
				bool does_have_attackers = (bishops != 0) || (ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::QUEEN)) != 0;
				if (does_have_attackers) {
					if (bit64(center_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_queen_side_square_with_qb += color == 0 ? 1 : -1;
					}
				}
				else {
					if (bit64(center_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_queen_side_square_without_qb += color == 0 ? 1 : -1;
					}
				}
				if (bit64(forwad_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
					queen_side_forward_square += color == 0 ? 1 : -1;
				}
			}

		}
		else if (king_file <= 2) {
			int forward_square = get_forward_square(king_squares[color], static_cast<Color>(color));
			if (forward_square >= 0 && forward_square < 64) {
				int center_square = forward_square + 1;
				int border_square = forward_square - 1;
				bool is_light_square = (bit64(center_square) & LIGHT_SQUARES) != 0;
				uint64_t ld_squares = is_light_square ? LIGHT_SQUARES : DARK_SQUARES;
				uint64_t bishops = ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::BISHOP) & ld_squares;
				bool does_have_attackers = (bishops != 0) || (ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::QUEEN)) != 0;
				if (does_have_attackers) {
					if (bit64(center_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_queen_side_square_with_qb += color == 0 ? 1 : -1;
					}
					if (bit64(border_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_queen_side_square_with_qb += color == 0 ? 1 : -1;
					}
				}
				else {
					if (bit64(center_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_queen_side_square_without_qb += color == 0 ? 1 : -1;
					}
					if (bit64(border_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_queen_side_square_without_qb += color == 0 ? 1 : -1;
					}
				}
				if (bit64(forward_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
					queen_side_forward_square += color == 0 ? 1 : -1;
				}
			}
		}
		else if (king_file <= 4) {
			int forward_square = get_forward_square(king_squares[color], static_cast<Color>(color));
			if (forward_square >= 0 && forward_square < 64) {
				int right_square = forward_square + 1;
				int left_square = forward_square - 1;
				bool is_light_square = (bit64(right_square) & LIGHT_SQUARES) != 0;
				uint64_t ld_squares = is_light_square ? LIGHT_SQUARES : DARK_SQUARES;
				uint64_t bishops = ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::BISHOP) & ld_squares;
				bool does_have_attackers = (bishops != 0) || (ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::QUEEN)) != 0;
				if (does_have_attackers) {
					if (bit64(right_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_central_square_with_qb += color == 0 ? 1 : -1;
					}
					if (bit64(left_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_central_square_with_qb += color == 0 ? 1 : -1;
					}
				}
				else {
					if (bit64(right_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_central_square_without_qb += color == 0 ? 1 : -1;
					}
					if (bit64(left_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_central_square_without_qb += color == 0 ? 1 : -1;
					}
				}
				if (bit64(forward_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
					center_forward_square += color == 0 ? 1 : -1;
				}
			}
		}
		else if (king_file <= 6) {
			int forward_square = get_forward_square(king_squares[color], static_cast<Color>(color));
			if (forward_square >= 0 && forward_square < 64) {
				int right_square = forward_square + 1;
				int left_square = forward_square - 1;
				bool is_light_square = (bit64(right_square) & LIGHT_SQUARES) != 0;
				uint64_t ld_squares = is_light_square ? LIGHT_SQUARES : DARK_SQUARES;
				uint64_t bishops = ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::BISHOP) & ld_squares;
				bool does_have_attackers = (bishops != 0) || (ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::QUEEN)) != 0;
				if (does_have_attackers) {
					if (bit64(right_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_king_side_square_with_qb += color == 0 ? 1 : -1;
					}
					if (bit64(left_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_king_side_square_with_qb += color == 0 ? 1 : -1;
					}
				}
				else {
					if (bit64(right_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						right_def_king_side_square_without_qb += color == 0 ? 1 : -1;
					}
					if (bit64(left_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_king_side_square_without_qb += color == 0 ? 1 : -1;
					}
				}
				if (bit64(forward_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
					king_side_forward_square += color == 0 ? 1 : -1;
				}
			}
		}
		else if (king_file == 7) {
			int forward_square = get_forward_square(king_squares[color], static_cast<Color>(color));
			if (forward_square >= 0 && forward_square < 64) {
				int left_square = forward_square - 1;
				bool is_light_square = (bit64(left_square) & LIGHT_SQUARES) != 0;
				uint64_t ld_squares = is_light_square ? LIGHT_SQUARES : DARK_SQUARES;
				uint64_t bishops = ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::BISHOP) & ld_squares;
				bool does_have_attackers = (bishops != 0) || (ctx.board.get_pieces(static_cast<Color>(op_color), PieceType::QUEEN)) != 0;
				if (does_have_attackers) {
					if (bit64(left_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_king_side_square_with_qb += color == 0 ? 1 : -1;
					}
				}
				else {
					if (bit64(left_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
						left_def_king_side_square_without_qb += color == 0 ? 1 : -1;
					}
				}
				if (bit64(forward_square) & ctx.board.get_pieces(static_cast<Color>(color), PieceType::PAWN)) {
					king_side_forward_square += color == 0 ? 1 : -1;
				}
			}
		}
		//Attacks around king
		uint64_t op_piece_attacks[6] = { 0 };
		op_piece_attacks[to_int(PieceType::QUEEN)] = ctx.board.get_queen_attacks_for_color(static_cast<Color>(op_color));
		op_piece_attacks[to_int(PieceType::ROOK)] = ctx.board.get_rook_attacks_for_color(static_cast<Color>(op_color));
		op_piece_attacks[to_int(PieceType::BISHOP)] = ctx.board.get_bishop_attacks_for_color(static_cast<Color>(op_color));
		op_piece_attacks[to_int(PieceType::KNIGHT)] = ctx.board.get_knight_attacks_for_color(static_cast<Color>(op_color));
		op_piece_attacks[to_int(PieceType::PAWN)] = ctx.board.get_pawn_attacks_for_color(static_cast<Color>(op_color));
		op_piece_attacks[to_int(PieceType::KING)] = ctx.board.get_king_attacks_for_color(static_cast<Color>(op_color));
		uint64_t op_all_attacks = op_piece_attacks[to_int(PieceType::QUEEN)] | op_piece_attacks[to_int(PieceType::ROOK)] |
			op_piece_attacks[to_int(PieceType::BISHOP)] | op_piece_attacks[to_int(PieceType::KNIGHT)] |
			op_piece_attacks[to_int(PieceType::PAWN)] | op_piece_attacks[to_int(PieceType::KING)];
		uint64_t small_mask = SMALL_KING_ZONE[king_squares[color]];
		uint64_t large_mask = KING_ZONE[king_squares[color]] & ~small_mask;
		int count = popcount(op_all_attacks & small_mask);
		if (count >= 7) count = 7;
		small_attack_count[count] += color == 0 ? 1 : -1;
		count = popcount(op_all_attacks & large_mask);
		if (count >= 12) count = 12;
		big_attack_count[count] += color == 0 ? 1 : -1;
		bool is_piece_attacking[6] = { false };
		is_piece_attacking[to_int(PieceType::QUEEN)] = (op_piece_attacks[to_int(PieceType::QUEEN)] & small_mask) != 0;
		is_piece_attacking[to_int(PieceType::ROOK)] = (op_piece_attacks[to_int(PieceType::ROOK)] & small_mask) != 0;
		is_piece_attacking[to_int(PieceType::BISHOP)] = (op_piece_attacks[to_int(PieceType::BISHOP)] & small_mask) != 0;
		is_piece_attacking[to_int(PieceType::KNIGHT)] = (op_piece_attacks[to_int(PieceType::KNIGHT)] & small_mask) != 0;
		is_piece_attacking[to_int(PieceType::PAWN)] = (op_piece_attacks[to_int(PieceType::PAWN)] & small_mask) != 0;
		is_piece_attacking[to_int(PieceType::KING)] = (op_piece_attacks[to_int(PieceType::KING)] & small_mask) != 0;
		int different_pieces_attacking = 0;
		for (PieceType pt : {PieceType::QUEEN, PieceType::ROOK, PieceType::BISHOP, PieceType::KNIGHT, PieceType::PAWN, PieceType::KING}) {
			if (is_piece_attacking[to_int(pt)]) {
				piece_attacking[to_int(pt)] += color == 0 ? 1 : -1;
				different_pieces_attacking++;
			}
		}
		if (different_pieces_attacking >= 4) different_pieces_attacking = 4;
		distinct_pieces_attacking[different_pieces_attacking] += color == 0 ? 1 : -1;
		uint64_t small_weak = small_mask & (ctx.backward[color] | ctx.isolated[color]);
		uint64_t large_weak = large_mask & (ctx.backward[color] | ctx.isolated[color]);
		weak_pawns_around_small_king += color == 0 ? popcount(small_weak) : -popcount(small_weak);
		weak_pawns_around_big_king += color == 0 ? popcount(large_weak) : -popcount(large_weak);
		op_pawns_in_small_mask += color == 0 ? popcount(ctx.get_pieces(op_color, PieceType::PAWN) & small_mask) : -popcount(ctx.get_pieces(op_color, PieceType::PAWN) & small_mask);
		op_pawns_in_big_mask += color == 0 ? popcount(ctx.get_pieces(op_color, PieceType::PAWN) & large_mask) : -popcount(ctx.get_pieces(op_color, PieceType::PAWN) & large_mask);
		int king_escape = popcount(small_mask & ~ctx.get_color_pieces(color) & ~op_all_attacks);
		if (king_escape >= 3) king_escape = 2;
		king_escape_squares[king_escape] += color == 0 ? 1 : -1;
		uint64_t queens = ctx.get_pieces(op_color, PieceType::QUEEN);
		if (queens != 0) {
			int king_queen_distance = king_distance(king_squares[color], get_lsb(queens)) - 1;
			king_tropism[king_queen_distance] += color == 0 ? 1 : -1;
		}
		int king_mobility = popcount(get_queen_attacks(king_squares[color], ctx.get_all_pieces()));
		mobility_penalty += color == 0 ? king_mobility : -king_mobility;

	}
	addTerm<isTracing>(score, EvalParam::KING_FILE_OPEN_WITH_RQ, king_file_open_with_rq, trace);
	addTerm<isTracing>(score, EvalParam::KING_FILE_OPEN_WITHOUT_RQ, king_file_open_without_rq, trace);
	addTerm<isTracing>(score, EvalParam::ADJ_FILE_OPEN_WITH_RQ, adj_file_open_with_rq, trace);
	addTerm<isTracing>(score, EvalParam::ADJ_FILE_OPEN_WITHOUT_RQ, adj_file_open_without_rq, trace);
	addTerm<isTracing>(score, EvalParam::NO_DEF_PAWNS_ON_KING_FILE_WITH_RQ, no_def_pawns_on_king_file_with_rq, trace);
	addTerm<isTracing>(score, EvalParam::NO_DEF_PAWNS_ON_KING_FILE_WITHOUT_RQ, no_def_pawns_on_king_file_without_rq, trace);
	addTerm<isTracing>(score, EvalParam::NO_ATT_PAWNS_ON_KING_FILE_WITH_RQ, no_att_pawns_on_king_file_with_rq, trace);
	addTerm<isTracing>(score, EvalParam::NO_ATT_PAWNS_ON_KING_FILE_WITHOUT_RQ, no_att_pawns_on_king_file_without_rq, trace);
	addTerm<isTracing>(score, EvalParam::FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITH_RQ, files_around_king_without_def_pawns_with_rq, trace);
	addTerm<isTracing>(score, EvalParam::FILES_AROUND_KING_WITHOUT_DEF_PAWNS_WITHOUT_RQ, files_around_king_without_def_pawns_without_rq, trace);
	addTerm<isTracing>(score, EvalParam::FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITH_RQ, files_around_king_without_att_pawns_with_rq, trace);
	addTerm<isTracing>(score, EvalParam::FILES_AROUND_KING_WITHOUT_ATT_PAWNS_WITHOUT_RQ, files_around_king_without_att_pawns_without_rq, trace);
	addTerm<isTracing>(score, EvalParam::PAWNS_DEFEND_DIAGONAL_START, pawns_defend_diagonal[0], trace);
	addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::PAWNS_DEFEND_DIAGONAL_START + 1), pawns_defend_diagonal[1], trace);
	addTerm<isTracing>(score, EvalParam::PAWNS_DEFEND_DIAGONAL_END, pawns_defend_diagonal[2], trace);
	addTerm<isTracing>(score, EvalParam::OTHER_DEF_COUNT_NO_PAWNS_START, other_def_count_no_pawns[0], trace);
	addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::OTHER_DEF_COUNT_NO_PAWNS_START + 1), other_def_count_no_pawns[1], trace);
	addTerm<isTracing>(score, EvalParam::OTHER_DEF_COUNT_NO_PAWNS_END, other_def_count_no_pawns[2], trace);
	addTerm<isTracing>(score, EvalParam::OTHER_DEF_COUNT_WITH_PAWNS_START, other_def_count_with_pawns[0], trace);
	addTerm<isTracing>(score, EvalParam::OTHER_DEF_COUNT_WITH_PAWNS_END, other_def_count_with_pawns[1], trace);
	addTerm<isTracing>(score, EvalParam::RIGHT_DEF_QUEEN_SIDE_SQUARE_WITH_QB, right_def_queen_side_square_with_qb, trace);
	addTerm<isTracing>(score, EvalParam::RIGHT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB, right_def_queen_side_square_without_qb, trace);
	addTerm<isTracing>(score, EvalParam::LEFT_DEF_QUEEN_SIDE_SQUARE_WITH_QB, left_def_queen_side_square_with_qb, trace);
	addTerm<isTracing>(score, EvalParam::LEFT_DEF_QUEEN_SIDE_SQUARE_WITHOUT_QB, left_def_queen_side_square_without_qb, trace);
	addTerm<isTracing>(score, EvalParam::LEFT_DEF_CENTRAL_SQUARE_WITH_QB, left_def_central_square_with_qb, trace);
	addTerm<isTracing>(score, EvalParam::LEFT_DEF_CENTRAL_SQUARE_WITHOUT_QB, left_def_central_square_without_qb, trace);
	addTerm<isTracing>(score, EvalParam::RIGHT_DEF_CENTRAL_SQUARE_WITH_QB, right_def_central_square_with_qb, trace);
	addTerm<isTracing>(score, EvalParam::RIGHT_DEF_CENTRAL_SQUARE_WITHOUT_QB, right_def_central_square_without_qb, trace);
	addTerm<isTracing>(score, EvalParam::CENTRAL_FORWARD_SQUARE, center_forward_square, trace);
	addTerm<isTracing>(score, EvalParam::LEFT_DEF_KING_SIDE_SQUARE_WITH_QB, left_def_king_side_square_with_qb, trace);
	addTerm<isTracing>(score, EvalParam::LEFT_DEF_KING_SIDE_SQUARE_WITHOUT_QB, left_def_king_side_square_without_qb, trace);
	addTerm<isTracing>(score, EvalParam::RIGHT_DEF_KING_SIDE_SQUARE_WITH_QB, right_def_king_side_square_with_qb, trace);
	addTerm<isTracing>(score, EvalParam::RIGHT_DEF_KING_SIDE_SQUARE_WITHOUT_QB, right_def_king_side_square_without_qb, trace);
	addTerm<isTracing>(score, EvalParam::QUEEN_SIDE_FORWARD_SQUARE, queen_side_forward_square, trace);
	addTerm<isTracing>(score, EvalParam::KING_SIDE_FORWARD_SQUARE, king_side_forward_square, trace);
	bool in_check = true;
	if (!in_check) {
		for (int i = 0; i < 8; i++) {
			addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::SMALL_ATTACK_COUNT_START + i), small_attack_count[i], trace);
		}
		for (int i = 0; i < 13; i++) {
			addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::BIG_ATTACK_COUNT_START + i), big_attack_count[i], trace);
		}
		for (int i = 0; i < 6; i++) {
			addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::PIECE_ATTACKING_START + i), piece_attacking[i], trace);
		}
		for (int i = 0; i < 5; i++) {
			addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::DISTINCT_PIECES_ATTACKING_START + i), distinct_pieces_attacking[i], trace);
		}
	}
	addTerm<isTracing>(score, EvalParam::WEAK_PAWNS_AROUND_SMALL_KING, weak_pawns_around_small_king, trace);
	addTerm<isTracing>(score, EvalParam::WEAK_PAWNS_AROUND_BIG_KING, weak_pawns_around_big_king, trace);
	addTerm<isTracing>(score, EvalParam::OP_PAWNS_IN_SMALL_MASK, op_pawns_in_small_mask, trace);
	addTerm<isTracing>(score, EvalParam::OP_PAWNS_IN_BIG_MASK, op_pawns_in_big_mask, trace);
	for (int i = 0; i < 3; i++) {
		addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::KING_ESCAPE_SQUARES_START + i), king_escape_squares[i], trace);
	}
	if (!in_check) {
		for (int i = 0; i < 6; i++) {
			addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::KING_TROPISM_START + i), king_tropism[i + 1], trace);
		}
	}
	addTerm<isTracing>(score, EvalParam::MOBILITY_PENALTY, mobility_penalty, trace);
}
template<bool isTracing>
void eval_mobility(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	for (PieceType pt : {PieceType::KNIGHT, PieceType::BISHOP, PieceType::ROOK, PieceType::QUEEN}) {
		int mob_count = 0;
		for (int color = 0; color < 2; color++) {
			int ecolor = color == 0 ? 1 : 0;
			uint64_t enemy_attacks = ctx.get_attacks(static_cast<Color>(ecolor));
			uint64_t pieces = ctx.get_pieces(color, pt);
			while (pieces) {
				int sq = get_lsb(pieces);
				if (color == 0)
					mob_count += popcount(get_piece_attacks(pt, sq, ctx.get_all_pieces()) & ~ctx.get_color_pieces(color) & ~enemy_attacks);
				else
					mob_count -= popcount(get_piece_attacks(pt, sq, ctx.get_all_pieces()) & ~ctx.get_color_pieces(color) & ~enemy_attacks);
				pieces &= pieces - 1;
			}
		}
		addTerm<isTracing>(score, static_cast<EvalParam>(EvalParam::MOBILITY_START + to_int(pt) - 1), mob_count, trace);
	}
}
template<bool isTracing>
void eval_rook_activity(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int open_count = 0;
	int semi_open_count = 0;
	int connected_rooks = 0;
	uint64_t no_white_pawns_files = 0;
	uint64_t no_black_pawns_files = 0;
	for (size_t file = 0; file < 8; file++) {
		if (!ctx.does_color_have_pawns_on_file(file, Color::WHITE)) no_white_pawns_files |= FILE_MASK[file];
		if (!ctx.does_color_have_pawns_on_file(file, Color::BLACK)) no_black_pawns_files |= FILE_MASK[file];

	}
	semi_open_count += popcount(ctx.get_pieces(0, PieceType::ROOK) & ~no_black_pawns_files & no_white_pawns_files) - popcount(ctx.get_pieces(1, PieceType::ROOK) & ~no_white_pawns_files & no_black_pawns_files);
	open_count += popcount(ctx.get_pieces(0, PieceType::ROOK) & no_black_pawns_files & no_white_pawns_files) - popcount(ctx.get_pieces(1, PieceType::ROOK) & no_white_pawns_files & no_black_pawns_files);

	int white_rook_square = get_lsb(ctx.get_pieces(0, PieceType::ROOK));
	int black_rook_square = get_lsb(ctx.get_pieces(1, PieceType::ROOK));
	uint64_t white_connected = get_rook_attacks(white_rook_square, ctx.get_all_pieces()) & ctx.get_pieces(0, PieceType::ROOK);
	uint64_t black_connected = get_rook_attacks(black_rook_square, ctx.get_all_pieces()) & ctx.get_pieces(1, PieceType::ROOK);
	connected_rooks += popcount(white_connected) - popcount(black_connected);
	addTerm<isTracing>(score, EvalParam::CONNECTED_ROOKS, connected_rooks, trace);
	addTerm<isTracing>(score, EvalParam::ROOK_ON_SEMI_OPEN_FILE, semi_open_count, trace);
	addTerm<isTracing>(score, EvalParam::ROOK_ON_OPEN_FILE, open_count, trace);
}

template<bool isTracing>
void eval_minor_pieces(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	eval_bishop_pair<isTracing>(score, ctx, trace);
	eval_bad_bishop<isTracing>(score, ctx, trace);
	eval_trapped_minor<isTracing>(score, ctx, trace);
	eval_fianchetto_bishop<isTracing>(score, ctx, trace);
	eval_outpost<isTracing>(score, ctx, trace);
}

template<bool isTracing>
void eval_bishop_pair(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int bishop_pair_count = 0;
	if (popcount(ctx.board.get_pieces(Color::WHITE, PieceType::BISHOP)) >= 2) bishop_pair_count++;
	if (popcount(ctx.board.get_pieces(Color::BLACK, PieceType::BISHOP)) >= 2) bishop_pair_count--;
	addTerm<isTracing>(score, EvalParam::BISHOP_PAIR, bishop_pair_count, trace);
}
template<bool isTracing>
void eval_bad_bishop(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int blocked_penalty_count = 0;
	int unblocked_penalty_count = 0;
	for (int color = 0; color < 2; color++) {
		uint64_t bishops = ctx.get_pieces(color, PieceType::BISHOP);

		while (bishops) {
			int sq = get_lsb(bishops);
			uint64_t bishop_color_mask = (bit64(sq) & LIGHT_SQUARES) != 0 ? LIGHT_SQUARES : DARK_SQUARES;


			// Zähle blockierte eigene Bauern auf der gleichen Farbe
			int blocked_pawns = 0;
			uint64_t pawns_to_check = ctx.get_pieces(color, PieceType::PAWN) & bishop_color_mask;

			while (pawns_to_check) {
				int pawn_sq = get_lsb(pawns_to_check);
				int forward_sq = (color == 0) ? pawn_sq + 8 : pawn_sq - 8;

				// Prüfe ob Bauer blockiert ist
				if (forward_sq >= 0 && forward_sq < 64 && (ctx.get_all_pieces() & (1ULL << forward_sq))) {
					blocked_penalty_count += color == 0 ? 1 : -1;
				}
				else {
					unblocked_penalty_count += color == 0 ? 1 : -1;
				}

				pawns_to_check &= pawns_to_check - 1;
			}

			bishops &= bishops - 1;
		}
	}
	addTerm<isTracing>(score, EvalParam::BAD_BISHOP_BLOCKED, blocked_penalty_count, trace);
	addTerm<isTracing>(score, EvalParam::BAD_BISHOP_UNBLOCKED, unblocked_penalty_count, trace);
}

template<bool isTracing>
void eval_trapped_minor(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int trapped_bishop_count = 0;
	int trapped_knight_count = 0;

	// Typische Fallen:
	// - Läufer auf a7/h7 (Weiß) oder a2/h2 (Schwarz) eingekesselt von Bauern
	// - Springer in der Ecke ohne Fluchtfelder

	for (int color = 0; color < 2; color++) {
		int ecolor = 1 - color;

		// Läufer-Fallen
		uint64_t bishops = ctx.get_pieces(color, PieceType::BISHOP);
		while (bishops) {
			int sq = get_lsb(bishops);

			// Läufer gefangen am Brettrand
			bool is_trapped = false;

			if (color == 0) {
				// a7-Falle: Läufer auf a7, Bauer auf b6
				if (sq == 48 && (ctx.get_pieces(ecolor, PieceType::PAWN) & (1ULL << 41))) is_trapped = true;
				// h7-Falle: Läufer auf h7, Bauer auf g6
				if (sq == 55 && (ctx.get_pieces(ecolor, PieceType::PAWN) & (1ULL << 46))) is_trapped = true;
			}
			else {
				// a2-Falle: Läufer auf a2, Bauer auf b3
				if (sq == 8 && (ctx.get_pieces(ecolor, PieceType::PAWN) & (1ULL << 17))) is_trapped = true;
				// h2-Falle: Läufer auf h2, Bauer auf g3
				if (sq == 15 && (ctx.get_pieces(ecolor, PieceType::PAWN) & (1ULL << 22))) is_trapped = true;
			}

			if (is_trapped) {
				trapped_bishop_count += color == 0 ? 1 : -1;
			}

			bishops &= bishops - 1;
		}

		// Springer-Fallen (Ecken mit blockierten Fluchtfeldern)
		uint64_t knights = ctx.get_pieces(color, PieceType::KNIGHT);
		while (knights) {
			int sq = get_lsb(knights);
			int file = sq % 8;
			int rank = sq / 8;

			// In Ecke und alle Fluchtfelder blockiert
			if ((file == 0 || file == 7) && (rank == 0 || rank == 7)) {
				uint64_t escape_squares = KNIGHT_ATTACKS[sq];
				int blocked_escapes = popcount(escape_squares & ctx.get_color_pieces(color));

				if (blocked_escapes >= 2) {  // Meiste Fluchtfelder blockiert
					trapped_knight_count += color == 0 ? 1 : -1;
				}
				else {
					bool is_trapped = true;
					uint64_t unoccupied_escapes = escape_squares & ~ctx.get_color_pieces(color);
					while (unoccupied_escapes) {
						int escape_sq = get_lsb(unoccupied_escapes);
						PieceType pt = ctx.get_piece_on_square(escape_sq);
						if (see_capture_ge(ctx.board, sq, escape_sq, static_cast<Color>(color), PieceType::KNIGHT, pt, 0)) {
							is_trapped = false;
							break;
						}
						unoccupied_escapes &= unoccupied_escapes - 1;
					}
					if (is_trapped) {
						trapped_knight_count += color == 0 ? 1 : -1;
					}
				}
			}

			knights &= knights - 1;
		}
	}
	addTerm<isTracing>(score, EvalParam::TRAPPED_BISHOP, trapped_bishop_count, trace);
	addTerm<isTracing>(score, EvalParam::TRAPPED_KNIGHT, trapped_knight_count, trace);
}

template<bool isTracing>
void eval_fianchetto_bishop(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int intact_count = 0;
	int broken_count = 0;

	// Fianchetto-Positionen: b2, g2 (Weiß), b7, g7 (Schwarz)
	const uint64_t WHITE_FIANCHETTO = (1ULL << 9) | (1ULL << 14);  // b2, g2
	const uint64_t BLACK_FIANCHETTO = (1ULL << 49) | (1ULL << 54); // b7, g7

	for (int color = 0; color < 2; color++) {
		uint64_t fianchetto_mask = (color == 0) ? WHITE_FIANCHETTO : BLACK_FIANCHETTO;
		uint64_t bishops_fianchetto = ctx.get_pieces(color, PieceType::BISHOP) & fianchetto_mask;

		while (bishops_fianchetto) {
			int sq = get_lsb(bishops_fianchetto);

			// Prüfe ob Bauernstruktur intakt ist (Bauer auf b3/g3 oder b6/g6)
			int pawn_sq = (color == 0) ? sq + 8 : sq - 8;
			bool pawn_structure_intact = (ctx.get_pieces(color, PieceType::PAWN) & (1ULL << pawn_sq)) != 0;

			if (pawn_structure_intact) {
				intact_count += (color == 0) ? 1 : -1;
			}
			else {
				// Strafe wenn Fianchetto-Bauer fehlt (schwacher König)
				broken_count += (color == 0) ? 1 : -1;
			}

			bishops_fianchetto &= bishops_fianchetto - 1;
		}
	}
	addTerm<isTracing>(score, EvalParam::FIANCHETTO_BISHOP, intact_count, trace);
	addTerm<isTracing>(score, EvalParam::BROKEN_FIANCHETTO, broken_count, trace);
}

template<bool isTracing>
void eval_outpost(EvaluationResult& score, const EvalContext& ctx, Trace* trace) {
	int bishop_outpost_count_no_op_bishop = 0;
	int bishop_outpost_count_with_op_bishop = 0;
	int knight_outpost_count_no_op_bishop = 0;
	int knight_outpost_count_with_op_bishop = 0;
	for (int color = 0; color < 2; color++) {
		int ecolor = color == 0 ? 1 : 0;
		uint64_t defended_by_own_pawn = get_pawn_attacks(ctx.get_pieces(color, PieceType::PAWN), static_cast<Color>(color));
		uint64_t out_post_mask = color == 0 ? WHITE_OUTPOST_MASK : BLACK_OUTPOST_MASK;
		uint64_t possible_bishop_outposts[2];
		uint64_t possible_knight_outposts[2];
		possible_bishop_outposts[0] = defended_by_own_pawn & out_post_mask & LIGHT_SQUARES & (ctx.get_pieces(color, PieceType::BISHOP));
		possible_bishop_outposts[1] = defended_by_own_pawn & out_post_mask & DARK_SQUARES & (ctx.get_pieces(color, PieceType::BISHOP));
		possible_knight_outposts[0] = defended_by_own_pawn & out_post_mask & LIGHT_SQUARES & (ctx.get_pieces(color, PieceType::KNIGHT));
		possible_knight_outposts[1] = defended_by_own_pawn & out_post_mask & DARK_SQUARES & (ctx.get_pieces(color, PieceType::KNIGHT));
		bool light_square_bishop_exists = (ctx.get_pieces(ecolor, PieceType::BISHOP) & LIGHT_SQUARES) != 0;
		bool dark_square_bishop_exists = (ctx.get_pieces(ecolor, PieceType::BISHOP) & DARK_SQUARES) != 0;
		auto count_outposts = [&](uint64_t outposts, bool enemy_bishop_exists, int& counter_no_op_bishop, int& counter_with_op_bishop) {
			while (outposts) {
				int square = get_lsb(outposts);
				if ((ctx.get_pieces(ecolor, PieceType::PAWN) & OUTPOST_MASK[color][square]) == 0) {
					if (enemy_bishop_exists) {
						counter_with_op_bishop += color == 0 ? 1 : -1;
					}
					else {
						counter_no_op_bishop += color == 0 ? 1 : -1;
					}
				}
				outposts &= outposts - 1;
			}
			};
		count_outposts(possible_bishop_outposts[0], light_square_bishop_exists, bishop_outpost_count_no_op_bishop, bishop_outpost_count_with_op_bishop);
		count_outposts(possible_bishop_outposts[1], dark_square_bishop_exists, bishop_outpost_count_no_op_bishop, bishop_outpost_count_with_op_bishop);
		count_outposts(possible_knight_outposts[0], light_square_bishop_exists, knight_outpost_count_no_op_bishop, knight_outpost_count_with_op_bishop);
		count_outposts(possible_knight_outposts[1], dark_square_bishop_exists, knight_outpost_count_no_op_bishop, knight_outpost_count_with_op_bishop);

	}
	addTerm<isTracing>(score, EvalParam::BISHOP_OUTPOST_NO_OPPOSITE_BISHOP, bishop_outpost_count_no_op_bishop, trace);
	addTerm<isTracing>(score, EvalParam::BISHOP_OUTPOST_WITH_OPPOSITE_BISHOP, bishop_outpost_count_with_op_bishop, trace);
	addTerm<isTracing>(score, EvalParam::KNIGHT_OUTPOST_NO_OPPOSITE_BISHOP, knight_outpost_count_no_op_bishop, trace);
	addTerm<isTracing>(score, EvalParam::KNIGHT_OUTPOST_WITH_OPPOSITE_BISHOP, knight_outpost_count_with_op_bishop, trace);
}
