// I'm starting to think that dumping all constants in one file wasn't the brightest idea...

#pragma once
#include <cstdint>

enum COLOR: int {
  WHITE,
  BLACK
};

enum PIECE: int {
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING
};

enum CASTLE: int {
  KING_SIDE,
  QUEEN_SIDE
};

enum MOVE: int {
  LEGAL,
  ILLEGAL,
  PSEUDO_LEGAL
};

const uint64_t SECOND_RANK  = 0x000000000000FF00ULL;
const uint64_t SEVENTH_RANK = 0x00FF000000000000ULL;

// Using "BOARD" prefix since "FILE" is causing a naming conflict with cstdio
enum BOARD_FILE: int {
  FILE_A = 1, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

enum BOARD_RANK: int {
  RANK_1 = 1, RANK_2, RANK_3, RANK_4, RANK_5,  RANK_6, RANK_7, RANK_8,
};


enum SQUARE: int {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8
};

BOARD_FILE get_file(SQUARE sq) { return BOARD_FILE(sq % 8); }
BOARD_RANK get_rank(SQUARE sq) { return BOARD_RANK(sq / 8); }
SQUARE make_square(BOARD_FILE f, BOARD_RANK r) { return SQUARE(r * 8 + f); }

const int NORTH = 8;
const int SOUTH = -8;
const int EAST = 1;
const int WEST = -1;

namespace FLAG {
  const uint16_t QUIET_MOVE = 0;
  const uint16_t CAPTURE = 1 << 12;
  const uint16_t PAWN_PROMOTION = 2 << 12;
  const uint16_t EN_PASSANT = 3 << 12;
  const uint16_t CASTLING = 4 << 12;
};
