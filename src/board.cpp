#include "../include/board.h"

Board::Board() {
  turn = COLOR::WHITE;
  set_player_color(COLOR::WHITE);
  setup_initial_position();
}

void Board::update_occupied() {
  occupied.clear_all();
  color_occupied[WHITE].clear_all();
  color_occupied[BLACK].clear_all();

  for (int color = 0; color < 2; ++color) {
    for (int piece = 0; piece < 6; ++piece) {
      occupied.OR(pieces[color][piece]);
      color_occupied[color].OR(pieces[color][piece]);
    }
  }
}

void Board::set_player_color(COLOR color) {
  player_color = color;
  engine_color = flip_color(color);
}

void Board::setup_initial_position() {
  pieces[COLOR::WHITE][PIECE::PAWN].set_bitboard(SECOND_RANK);
  pieces[COLOR::BLACK][PIECE::PAWN].set_bitboard(SEVENTH_RANK);

  pieces[COLOR::WHITE][PIECE::KNIGHT].set_bit(SQUARE::B1);
  pieces[COLOR::WHITE][PIECE::KNIGHT].set_bit(SQUARE::G1);
  pieces[COLOR::BLACK][PIECE::KNIGHT].set_bit(SQUARE::B8);
  pieces[COLOR::BLACK][PIECE::KNIGHT].set_bit(SQUARE::G8);

  pieces[COLOR::WHITE][PIECE::BISHOP].set_bit(SQUARE::C1);
  pieces[COLOR::WHITE][PIECE::BISHOP].set_bit(SQUARE::F1);
  pieces[COLOR::BLACK][PIECE::BISHOP].set_bit(SQUARE::C8);
  pieces[COLOR::BLACK][PIECE::BISHOP].set_bit(SQUARE::F8);

  pieces[COLOR::WHITE][PIECE::ROOK].set_bit(SQUARE::A1);
  pieces[COLOR::WHITE][PIECE::ROOK].set_bit(SQUARE::H1);
  pieces[COLOR::BLACK][PIECE::ROOK].set_bit(SQUARE::A8);
  pieces[COLOR::BLACK][PIECE::ROOK].set_bit(SQUARE::H8);

  pieces[COLOR::WHITE][PIECE::QUEEN].set_bit(SQUARE::D1);
  pieces[COLOR::BLACK][PIECE::QUEEN].set_bit(SQUARE::D8);

  pieces[COLOR::WHITE][PIECE::KING].set_bit(SQUARE::E1);
  pieces[COLOR::BLACK][PIECE::KING].set_bit(SQUARE::E8);
}

std::array<std::array<Bitboard, 6>, 2> Board::get_pieces() { return pieces; }

Bitboard Board::get_piece(COLOR color, PIECE piece_type) {
  return pieces[color][piece_type];
}

Bitboard Board::get_occupied() { return occupied; }

Bitboard Board::get_color_occupied(COLOR color) {
  return color_occupied[color];
}

COLOR Board::get_color() { return turn; }

std::vector<Move> Board::get_move_list() { return move_list; }

COLOR Board::flip_color(COLOR color) { return static_cast<COLOR>(~color); }

bool Board::can_castle(COLOR color, CASTLE castle_type) {
  return (castling & (1ULL << (castle_type + (color == COLOR::BLACK ? 2 : 0))));
}

void Board::next_turn() { turn = flip_color(turn); }

void Board::set_turn(COLOR color) { turn = color; }

void Board::increment_reversible_moves() { reversible_moves += 1; }

void Board::reset_reversible_moves() { reversible_moves = 0; }

void Board::invalidate_castling(COLOR color, CASTLE castle_type) {
  castling &= ~(1ULL << (castle_type + (color == COLOR::BLACK ? 2 : 0)));
}

void Board::clear_en_passant_square() {
  en_passant_square = SQUARE::NO_SQUARE;
}

SQUARE Board::get_en_passant_square() const {
  return en_passant_square;
}

void Board::set_en_passant_square(SQUARE square) {
  en_passant_square = square;
}

void Board::make_move(Move move) {
  COLOR moving_color = turn;
  SQUARE origin = move.get_origin();
  SQUARE target = move.get_target();
  uint16_t flags = move.get_flags();

  PIECE moving_piece;
  for (int piece = PIECE::PAWN; piece <= PIECE::KING; ++piece) {
    if (pieces[moving_color][piece].is_occupied(origin)) {
      moving_piece = static_cast<PIECE>(piece);
      break;
    }
  }

  if (moving_piece == PIECE::PAWN || (flags & FLAG::CAPTURE)) {
    reset_reversible_moves();
  } else {
    increment_reversible_moves();
  }

  pieces[moving_color][moving_piece].clear_bit(origin);

  if (move.is_capture()) {
    COLOR enemy_color = flip_color(moving_color);
    // For en passant, the captured piece isn't on the target square
    SQUARE capture_square = target;
    if (move.is_en_passant()) {
      capture_square = static_cast<SQUARE>(target - (moving_color == COLOR::WHITE ? 8 : -8));
    }

    for (int piece = PIECE::PAWN; piece <= PIECE::KING; ++piece) {
      if (pieces[enemy_color][piece].is_occupied(capture_square)) {
        pieces[enemy_color][piece].clear_bit(capture_square);
        break;
      }
    }
  }

  if (moving_piece == PIECE::PAWN) {
    if (move.is_promotion()) {
      if (move.is_promotion_knight()) moving_piece = PIECE::KNIGHT;
      else if (move.is_promotion_bishop()) moving_piece = PIECE::BISHOP;
      else if (move.is_promotion_rook()) moving_piece = PIECE::ROOK;
      else moving_piece = PIECE::QUEEN;
    }

    if (move.is_double_pawn_push()) {
      set_en_passant_square(static_cast<SQUARE>(target - (moving_color == COLOR::WHITE ? 8 : -8)));
    } else {
      clear_en_passant_square();
    }
  } else {
    clear_en_passant_square();
  }

  pieces[moving_color][moving_piece].set_bit(target);

  if (move.is_castling()) {
    BOARD_FILE rook_origin_file = (move.is_king_side_castling()) ? FILE_H : FILE_A;
    BOARD_FILE rook_target_file = (move.is_king_side_castling()) ? FILE_F : FILE_D;
    BOARD_RANK rank = (moving_color == COLOR::WHITE) ? RANK_1 : RANK_8;

    SQUARE rook_origin = make_square(rook_origin_file, rank);
    SQUARE rook_target = make_square(rook_target_file, rank);

    pieces[moving_color][PIECE::ROOK].clear_bit(rook_origin);
    pieces[moving_color][PIECE::ROOK].set_bit(rook_target);

    invalidate_castling(moving_color, CASTLE::KING_SIDE);
    invalidate_castling(moving_color, CASTLE::QUEEN_SIDE);
  }

  if (moving_piece == PIECE::KING) {
    invalidate_castling(moving_color, CASTLE::KING_SIDE);
    invalidate_castling(moving_color, CASTLE::QUEEN_SIDE);
  } else if (moving_piece == PIECE::ROOK) {
    if (origin == make_square(FILE_A, moving_color == COLOR::WHITE ? RANK_1 : RANK_8)) {
      invalidate_castling(moving_color, CASTLE::QUEEN_SIDE);
    } else if (origin == make_square(FILE_H, moving_color == COLOR::WHITE ? RANK_1 : RANK_8)) {
      invalidate_castling(moving_color, CASTLE::KING_SIDE);
    }
  }

  // If we capture a rook in its original position, remove castling rights for that side
  if (move.is_capture()) {
    COLOR enemy_color = flip_color(moving_color);
    if (target == make_square(FILE_A, enemy_color == COLOR::WHITE ? RANK_1 : RANK_8)) {
      invalidate_castling(enemy_color, CASTLE::QUEEN_SIDE);
    } else if (target == make_square(FILE_H, enemy_color == COLOR::WHITE ? RANK_1 : RANK_8)) {
      invalidate_castling(enemy_color, CASTLE::KING_SIDE);
    }
  }

  update_occupied();
  next_turn();
  move_list.push_back(move);
}
