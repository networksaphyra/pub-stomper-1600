#include "../include/movetable.h"

MoveTable::MoveTable() {
  init_knight_move_table();
  init_bishop_move_table();
  init_rook_move_table();
  init_queen_move_table();
}

void MoveTable::init_knight_move_table() {
  for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
    Bitboard moves;
    BOARD_FILE file = get_file(square);
    BOARD_RANK rank = get_rank(square);

    for (const auto &[delta_rank, delta_file] : directions[PIECE::KNIGHT]) {
      BOARD_FILE new_file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
      BOARD_RANK new_rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_rank);

      if (is_in_bounds(new_file, new_rank)) {
        moves.set_bit(make_square(new_file, new_rank));
      }
    }
    move_table[PIECE::KNIGHT][square].set_bitboard(moves);
  }
}

void MoveTable::init_bishop_move_table() {
  for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
    Bitboard moves;
    for (const auto &[delta_rank, delta_file] : directions[PIECE::BISHOP]) {
      Bitboard ray_attacks = get_ray_attacks(square, delta_rank, delta_file);
      moves.OR(ray_attacks);
    }
    move_table[PIECE::BISHOP][square].set_bitboard(moves);
  }
}

void MoveTable::init_rook_move_table() {
  for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
    Bitboard moves;
    for (const auto &[delta_rank, delta_file] : directions[PIECE::ROOK]) {
      Bitboard ray_attacks = get_ray_attacks(square, delta_rank, delta_file);
      moves.OR(ray_attacks);
    }
    move_table[PIECE::ROOK][square].set_bitboard(moves);
  }
}

void MoveTable::init_queen_move_table() {
  for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
    move_table[PIECE::QUEEN][square].set_bitboard(
        move_table[PIECE::BISHOP][square].GET_OR(move_table[PIECE::ROOK][square])
    );
  }
}

std::array<Bitboard, 64> MoveTable::get_move_table(PIECE piece) {
  return move_table[piece];
}
Bitboard MoveTable::get_moves(PIECE piece, SQUARE square) {
  return move_table[piece][square];
}
