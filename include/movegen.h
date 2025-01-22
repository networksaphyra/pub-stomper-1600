#pragma once

#include <vector>
#include "bitboard.h"
#include "constants.h"
#include "board.h"
#include "move.h"
#include "movetable.h"

class MoveGenerator {
private:
  MoveTable move_table;
  std::vector<Move> generate_sliding_moves(Board& board, PIECE piece);
  std::vector<Move> generate_non_sliding_moves(Board& board, PIECE piece);
  std::vector<Move> generate_pawn_moves(Board& board);
  std::vector<Move> generate_castling_moves(Board& board);
  bool is_square_attacked(Board& board, SQUARE square, COLOR attacker_color);
  bool is_check(Board& board, COLOR color);
  bool is_move_legal(Board& board, const Move& move);
public:
  std::vector<Move> generate_legal_moves(Board& board);
  std::vector<Move> generate_piece_moves(Board& board, PIECE piece);
};
