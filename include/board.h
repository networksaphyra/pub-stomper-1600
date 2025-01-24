#pragma once

#include "bitboard.h"
#include "constants.h"
#include "move.h"
#include <array>
#include <cstdint>
#include <vector>

class Board {
public:
  Board();
  Bitboard get_piece(COLOR color, PIECE piece_type);
  std::pair<COLOR, PIECE> get_piece_at(SQUARE square);
  std::pair<COLOR, PIECE> get_piece_at(BOARD_FILE file, BOARD_RANK rank);

  Bitboard get_occupied();
  Bitboard get_color_occupied(COLOR color);

  COLOR get_color();
  std::vector<Move> get_move_list();
  COLOR flip_color(COLOR color);

  bool can_castle(COLOR color, CASTLE castle_type);
  void next_turn();
  void set_turn(COLOR color);
  void increment_reversible_moves();
  void reset_reversible_moves();
  void invalidate_castling(COLOR color, CASTLE castle_type);

  SQUARE get_en_passant_square() const;
  void set_en_passant_square(SQUARE square);
  void clear_en_passant_square();

  void make_move(const Move move);

private:
  Bitboard occupied;
  std::array<Bitboard, 2> color_occupied;
  std::array<std::array<Bitboard, 6>, 2> pieces;
  COLOR player_color, engine_color;
  COLOR turn;
  int reversible_moves = 0;
  std::vector<Move> move_list;
  uint8_t castling = 0b1111;
  SQUARE en_passant_square = SQUARE::NO_SQUARE;

  void update_occupied();
  void set_player_color(COLOR color);
  void setup_initial_position();
};
