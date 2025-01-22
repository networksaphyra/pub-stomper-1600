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

bool Move::is_promotion_knight() { return flags & FLAG::PROMOTION_KNIGHT; }

bool Move::is_promotion_bishop() { return flags & FLAG::PROMOTION_BISHOP; }

bool Move::is_promotion_rook() { return flags & FLAG::PROMOTION_ROOK; }

bool Move::is_promotion_queen() { return flags & FLAG::PROMOTION_QUEEN; }

bool Move::is_double_pawn_push() { return flags & FLAG::DOUBLE_PAWN_PUSH; }

bool Move::is_en_passant() { return flags & FLAG::EN_PASSANT; }

bool Move::is_castling() { return flags & FLAG::CASTLING; }

bool Move::is_king_side_castling() { return flags & FLAG::KING_CASTLE; }

bool Move::is_queen_side_castling() { return flags & FLAG::QUEEN_CASTLE; }
