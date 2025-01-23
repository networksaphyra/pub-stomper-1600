#pragma once

#include "bitboard.h"
#include "constants.h"
#include <array>

class MoveTable {
public:
  MoveTable();
  std::array<Bitboard, 64> get_move_table(PIECE piece);
  Bitboard get_moves(PIECE piece, SQUARE square);

private:
  std::array<std::array<Bitboard, 64>, 5> move_table;
  void init_knight_move_table();
  void init_bishop_move_table();
  void init_rook_move_table();
  void init_queen_move_table();
  void init_king_move_table();
};
