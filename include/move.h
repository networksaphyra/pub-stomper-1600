#pragma once

#include "constants.h"
#include <cstdint>

class Move {
public:
  Move(SQUARE origin, SQUARE target, uint16_t flags = FLAG::NONE);
  SQUARE get_origin();
  SQUARE get_target();
  uint16_t get_flags();

  bool is_capture();
  bool is_promotion();
  bool is_en_passant();
  bool is_castling();

private:
  SQUARE origin;
  SQUARE target;
  uint16_t flags;
};
