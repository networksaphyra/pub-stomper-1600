#pragma once

#include "../include/constants.h"
#include "../include/bitboard.h"

BOARD_FILE get_file(SQUARE square) { return BOARD_FILE(square % 8); }
BOARD_RANK get_rank(SQUARE square) { return BOARD_RANK(square / 8); }
SQUARE make_square(BOARD_FILE file, BOARD_RANK rank) { return SQUARE(rank * 8 + file); }

Bitboard get_ray_attacks(SQUARE square, int delta_rank, int delta_file) {
  Bitboard attacks;
  BOARD_FILE file = get_file(square);
  BOARD_RANK rank = get_rank(square);

  file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
  rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_rank);

  while (is_in_bounds(file, rank)) {
    attacks.set_bit(make_square(file, rank));
    file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
    rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_rank);
  }

  return attacks;
}

bool is_in_bounds(BOARD_FILE file, BOARD_RANK rank) {
  return file >= FILE_A && file <= FILE_H && rank >= RANK_1 && rank <= RANK_8;
}
