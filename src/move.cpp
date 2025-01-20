#pragma once
#include "constants.cpp"
#include <cstdint>

class Move {
private:
  SQUARE origin;
  SQUARE target;
  uint16_t flags;

public:
  Move(SQUARE origin, SQUARE target, uint16_t flags = FLAG::NONE):
  origin(origin),
  target(target),
  flags(flags) {}

  SQUARE get_origin() { return origin; }
  SQUARE get_target() { return target; }
  uint16_t get_flags() { return flags; }

  bool is_capture() { return flags & FLAG::CAPTURE; }
  bool is_promotion() { return flags & FLAG::PAWN_PROMOTION; }
  bool is_en_passant() { return flags & FLAG::EN_PASSANT; }
  bool is_castling() { return flags & FLAG::CASTLING; }
};
