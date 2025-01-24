#include "../include/evaluate.h"
#include <iostream>

int Evaluate::get_piece_value(COLOR color, PIECE piece, Board& board) {
  if (piece == PIECE::NO_PIECE) return 0;
  return board.get_piece(color, piece).count_bits() * PIECE_VALUE[piece] * (color == COLOR::WHITE ? 1 : -1);
};

// for black, the square at a8 should be equivalent to a1
// and the value of the square should be reverse

int Evaluate::get_piece_square_value(COLOR color, PIECE piece, SQUARE square) {
  SQUARE table_square = color == COLOR::WHITE ? static_cast<SQUARE>(63 - square) : square;

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
        int piece_value = get_piece_value(color, piece, board);
        int square_value = get_piece_square_value(color, piece, square);

        eval += piece_value + square_value;
      }
    }
  }

  return eval;
}
