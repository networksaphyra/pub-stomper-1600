#pragma once
#include "bitboard.cpp"
#include "board.cpp"
#include "move.cpp"
#include <array>
#include <vector>

class MoveGenerator {
private:
  std::array<Bitboard, 64> pawn_move_table;
  std::array<Bitboard, 64> knight_move_table;
  std::array<Bitboard, 64> bishop_move_table;
  std::array<Bitboard, 64> rook_move_table;
  std::array<Bitboard, 64> queen_move_table;
  std::array<Bitboard, 64> king_move_table;

  void init_move_tables() { init_knight_move_table(); }

  // This feels very overengineered.
  void init_knight_move_table() {
    const std::array<std::pair<int, int>, 8> knight_deltas = {{
        {2, 1},
        {1, 2},
        {-1, 2},
        {-2, 1},
        {-2, -1},
        {-1, -2},
        {1, -2},
        {2, -1}
    }};

    for (SQUARE sq = static_cast<SQUARE>(0); sq < H8; sq = static_cast<SQUARE>(sq + 1)) {
      Bitboard moves;
      BOARD_FILE file = get_file(sq);
      BOARD_RANK rank = get_rank(sq);

      for (const auto &[delta_file, delta_rank] : knight_deltas) {
        BOARD_FILE new_file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
        BOARD_RANK new_rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_file);

        if (new_file >= FILE_A && new_file <= FILE_H && new_rank >= RANK_1 && new_rank <= RANK_8) {
          SQUARE target = make_square(new_file, new_rank);
          moves.set_bit(target);
        }
      }

      knight_move_table[sq] = moves;
    }
  }

public:
  MoveGenerator() { init_move_tables(); }

  std::vector<Move> generate_legal_moves(Board &board) {
    std::vector<Move> legal_moves;
    return legal_moves;
  }

private:
  std::vector<Move> generate_knight_moves(Board &board) {}
};
