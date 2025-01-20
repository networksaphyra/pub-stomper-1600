// I'm starting to think that dumping all constants in one file wasn't the brightest idea...

#pragma once
#include <cstdint>
#include <vector>

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

enum LEGALITY: int {
  LEGAL,
  ILLEGAL,
  PSEUDO_LEGAL
};

const uint64_t SECOND_RANK  = 0x000000000000FF00ULL;
const uint64_t SEVENTH_RANK = 0x00FF000000000000ULL;

// Using "BOARD" prefix since "FILE" is causing a naming conflict with cstdio
enum BOARD_FILE: int {
  FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

enum BOARD_RANK: int {
  RANK_1, RANK_2, RANK_3, RANK_4, RANK_5,  RANK_6, RANK_7, RANK_8,
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

const int BOARD_NORTH = 8;
const int BOARD_SOUTH = -8;
const int BOARD_EAST  = 1;
const int BOARD_WEST  = -1;

const int NORTH    = -1;
const int SOUTH  =  1;
const int EAST =  1;
const int WEST  = -1;

namespace FLAG {
  const uint16_t NONE = 0;
  const uint16_t CAPTURE = 1 << 12;
  const uint16_t PAWN_PROMOTION = 2 << 12;
  const uint16_t EN_PASSANT = 3 << 12;
  const uint16_t CASTLING = 4 << 12;
};

const std::vector<std::pair<int, int>> knight_deltas = {{
  {2 * NORTH, EAST}, {2 * NORTH, WEST}, {2 * SOUTH, EAST}, {2 * SOUTH, WEST},
  {NORTH, 2 * EAST}, {NORTH, 2 * WEST}, {SOUTH, 2 * EAST}, {SOUTH, 2 * WEST}
}};
const std::vector<std::pair<int, int>> bishop_deltas = {{
  {NORTH, EAST}, {NORTH, WEST}, {SOUTH, EAST}, {SOUTH, WEST}
}};
const std::vector<std::pair<int, int>> rook_deltas = {
  {NORTH, 0}, {SOUTH, 0}, {0, EAST}, {0, WEST}
};
const std::vector<std::pair<int, int>> queen_deltas = {
  {NORTH, EAST}, {NORTH, WEST}, {SOUTH, EAST}, {SOUTH, WEST},
  {NORTH, 0}, {SOUTH, 0}, {0, EAST}, {0, WEST}
};
