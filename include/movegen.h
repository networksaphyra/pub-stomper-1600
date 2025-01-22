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
public:
  std::vector<Move> generate_legal_moves(const Board& board);
  std::vector<Move> generate_piece_moves(Board& board, PIECE piece);
};
