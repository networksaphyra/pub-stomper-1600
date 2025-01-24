#pragma once

#include "constants.h"
#include <cstdint>

class Move {
public:
  Move(SQUARE origin, SQUARE target, uint16_t flags = FLAG::NONE, std::pair<COLOR, PIECE> captured_pice = no_piece);
  SQUARE get_origin();
  SQUARE get_target();
  uint16_t get_flags();

  bool is_capture();
  bool is_promotion();
  bool is_promotion_knight();
  bool is_promotion_bishop();
  bool is_promotion_rook();
  bool is_promotion_queen();
  bool is_double_pawn_push();
  bool is_en_passant();
  bool is_castling();
  bool is_king_side_castling();
  bool is_queen_side_castling();

private:
  SQUARE origin;
  SQUARE target;
  uint16_t flags;
  std::pair<COLOR, PIECE> captured_piece = no_piece;
};
