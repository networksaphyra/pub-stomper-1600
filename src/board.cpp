#include "../include/board.h"

Board::Board() {
  turn = COLOR::WHITE;
  set_player_color(COLOR::WHITE);
  setup_initial_position();
  reversible_moves = 0;
  castling = 0b1111;
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

COLOR Board::flip_color(COLOR color) { return static_cast<COLOR>(~color); }

bool Board::can_castle(COLOR color, CASTLE castle_type) {
  return (castling & (1ULL << (castle_type + (color == COLOR::BLACK ? 2 : 0))));
}

void Board::next_turn() { turn = flip_color(turn); }

void Board::set_reversible_moves(int moves) { reversible_moves = moves; }

void Board::invalidate_castling(COLOR color, CASTLE castle_type) {
  castling &= ~(1ULL << (castle_type + (color == COLOR::BLACK ? 2 : 0)));
}
