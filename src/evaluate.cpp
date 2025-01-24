#include "../include/evaluate.h"

int Evaluate::get_piece_value(COLOR color, PIECE piece) {
  if (piece == PIECE::NO_PIECE) return 0;
  return PIECE_VALUE[piece] * (color == COLOR::WHITE ? 1 : -1);
};

// for black, the square at a8 should be equivalent to a1
// and the value of the square should be reversed

int Evaluate::get_piece_square_value(COLOR color, PIECE piece, SQUARE square) {
  SQUARE table_square = static_cast<SQUARE>(63 - square);

  if (color == COLOR::BLACK) {
    BOARD_RANK rank = get_rank(table_square);
    table_square = make_square(get_file(table_square), static_cast<BOARD_RANK>(7 - rank));
  }

  int square_value = get_square_value(piece, table_square);
  return square_value * (color == COLOR::WHITE ? 1 : -1);
}

int Evaluate::evaluate(Board& board) {
  int eval = 0;

  for (COLOR color = COLOR::WHITE; color <= COLOR::BLACK; color = static_cast<COLOR>(color + 1)) {
    for (PIECE piece = PIECE::PAWN; piece <= PIECE::KING; piece = static_cast<PIECE>(piece + 1)) {
      Bitboard piece_bb = board.get_piece(color, piece);
      while (piece_bb.count_bits()) {
        SQUARE square = static_cast<SQUARE>(piece_bb.pop_least_significant_bit());
        eval += get_piece_value(color, piece);
        eval += get_piece_square_value(color, piece, square);
      }
    }
  }

  return eval;
}
