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

  bool is_in_bounds(BOARD_FILE file, BOARD_RANK rank) {
    return file >= FILE_A && file <= FILE_H && rank >= RANK_1 && rank <= RANK_8;
  }

  Bitboard get_ray_attacks(SQUARE square, int delta_rank, int delta_file) {
    Bitboard attacks;
    BOARD_FILE file = get_file(square);
    BOARD_RANK rank = get_rank(square);

    file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
    rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_rank);

    while (is_in_bounds(file, rank)) {
      attacks.set_bit(make_square(file, rank));
      file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
      rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_rank);
    }

    return attacks;
  }

  void init_move_tables() {
    init_knight_move_table();
    init_bishop_move_table();
    init_rook_move_table();
    init_queen_move_table();
  }

  void init_knight_move_table() {
    const std::array<std::pair<int, int>, 8> knight_deltas = {{
      {2 * NORTH, EAST}, {2 * NORTH, WEST}, {2 * SOUTH, EAST}, {2 * SOUTH, WEST},
      {NORTH, 2 * EAST}, {NORTH, 2 * WEST}, {SOUTH, 2 * EAST}, {SOUTH, 2 * WEST}
    }};

    for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
      Bitboard moves;
      BOARD_FILE file = get_file(square);
      BOARD_RANK rank = get_rank(square);

      for (const auto &[delta_rank, delta_file] : knight_deltas) {
        BOARD_FILE new_file = static_cast<BOARD_FILE>(static_cast<int>(file) + delta_file);
        BOARD_RANK new_rank = static_cast<BOARD_RANK>(static_cast<int>(rank) + delta_rank);

        if (is_in_bounds(new_file, new_rank)) {
          moves.set_bit(make_square(new_file, new_rank));
        }
      }

      knight_move_table[square] = moves;
    }
  }

  void init_bishop_move_table() {
    for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
      Bitboard moves;
      for (const auto &[delta_rank, delta_file] : bishop_deltas) {
        moves.OR(get_ray_attacks(square, delta_rank, delta_file));
      }
      bishop_move_table[square] = moves;
    }
  }

  void init_rook_move_table() {
    const std::array<std::pair<int, int>, 4> rook_deltas = {{
      {NORTH, 0}, {SOUTH, 0}, {0, EAST}, {0, WEST}
    }};

    for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
      Bitboard moves;
      for (const auto &[delta_rank, delta_file] : rook_deltas) {
        moves.OR(get_ray_attacks(square, delta_rank, delta_file));
      }
      rook_move_table[square] = moves;
    }
  }

  void init_queen_move_table() {
    for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
      queen_move_table[square].set_bitboard(
        bishop_move_table[square].GET_OR(rook_move_table[square]));
    }
  }

  std::vector<Move> generate_sliding_moves(Board &board, PIECE piece_type,
                                         const std::vector<std::pair<int, int>> &directions,
                                         const std::array<Bitboard, 64> &move_table) {
    std::vector<Move> moves;
    COLOR color = board.get_color();
    Bitboard pieces = board.get_piece(color, piece_type);
    Bitboard friendly = board.get_color_occupied(color);
    Bitboard enemy = board.get_color_occupied(board.flip_color(color));
    Bitboard occupied = board.get_occupied();

    while (pieces.get_bitboard()) {
      SQUARE origin = static_cast<SQUARE>(pieces.get_least_significant_bit());
      Bitboard moves_bb = move_table[origin];

      for (const auto &[delta_rank, delta_file] : directions) {
        Bitboard ray = get_ray_attacks(origin, delta_rank, delta_file);
        Bitboard blockers = ray.GET_AND(occupied);

        if (blockers.get_bitboard()) {
          SQUARE blocker = static_cast<SQUARE>(blockers.get_least_significant_bit());
          ray.XOR(get_ray_attacks(blocker, delta_rank, delta_file));
        }
        moves_bb.AND(ray);
      }

      moves_bb.AND(friendly.GET_NOT());

      while (moves_bb.get_bitboard()) {
        SQUARE target = static_cast<SQUARE>(moves_bb.get_least_significant_bit());
        moves.push_back(Move(origin, target, enemy.is_occupied(target) ? FLAG::CAPTURE : FLAG::NONE));
        moves_bb.clear_bit(target);
      }
      pieces.clear_bit(origin);
    }
    return moves;
  }

public:
  MoveGenerator() {
    init_move_tables();
  }

  // todo: filter illegal moves which put the king in check
  std::vector<Move> generate_legal_moves(const Board &board) {
    std::vector<Move> legal_moves;
    return legal_moves;
  }

  std::vector<Move> generate_knight_moves(Board &board) {
    std::vector<Move> moves;
    COLOR color = board.get_color();
    Bitboard knights = board.get_piece(color, PIECE::KNIGHT);
    Bitboard friendly = board.get_color_occupied(color);
    Bitboard enemy = board.get_color_occupied(board.flip_color(color));

    while (knights.get_bitboard()) {
      SQUARE origin = static_cast<SQUARE>(knights.get_least_significant_bit());
      Bitboard attack = knight_move_table[origin];
      attack.AND(friendly.GET_NOT());

      while (attack.get_bitboard()) {
        SQUARE target = static_cast<SQUARE>(attack.get_least_significant_bit());
        moves.push_back(Move(origin, target, enemy.is_occupied(target) ? FLAG::CAPTURE : FLAG::NONE));
        attack.clear_bit(target);
      }
      knights.clear_bit(origin);
    }
    return moves;
  }

  std::vector<Move> generate_bishop_moves(Board &board) {
    return generate_sliding_moves(board, PIECE::BISHOP, bishop_deltas, bishop_move_table);
  }

  std::vector<Move> generate_rook_moves(Board &board) {
    return generate_sliding_moves(board, PIECE::ROOK, rook_deltas, rook_move_table);
  }

  std::vector<Move> generate_queen_moves(Board &board) {
    return generate_sliding_moves(board, PIECE::QUEEN, queen_deltas, queen_move_table);
  }
};
