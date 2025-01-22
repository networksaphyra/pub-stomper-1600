#include "../include/move.h"

Move::Move(SQUARE origin, SQUARE target, uint16_t flags):
  origin(origin),
  target(target),
  flags(flags) {}

SQUARE Move::get_origin() { return origin; }

SQUARE Move::get_target() { return target; }

uint16_t Move::get_flags() { return flags; }

bool Move::is_capture() { return flags & FLAG::CAPTURE; }

bool Move::is_promotion() { return flags & FLAG::PAWN_PROMOTION; }

bool Move::is_en_passant() { return flags & FLAG::EN_PASSANT; }

bool Move::is_castling() { return flags & FLAG::CASTLING; }
