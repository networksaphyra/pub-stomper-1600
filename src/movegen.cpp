#pragma once
#include "bitboard.cpp"
#include "board.cpp"
#include "move.cpp"
#include "movetable.cpp"
#include <array>
#include <vector>

class MoveGenerator {
private:
  MoveTable move_table;

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

  std::vector<Move> generate_sliding_moves(Board &board, PIECE piece) {
    std::vector<Move> moves;
    COLOR color = board.get_color();
    Bitboard pieces = board.get_piece(color, piece);
    Bitboard friendly = board.get_color_occupied(color);
    Bitboard enemy = board.get_color_occupied(board.flip_color(color));
    Bitboard occupied = board.get_occupied();

    while (pieces.get_bitboard()) {
      SQUARE origin = static_cast<SQUARE>(pieces.get_least_significant_bit());
      Bitboard moves_bb = move_table.get_moves(piece, origin);

      for (const auto &[delta_rank, delta_file] : directions[piece]) {
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

  std::vector<Move> generate_non_sliding_moves(Board &board, PIECE piece) {
    std::vector<Move> moves;
    COLOR color = board.get_color();
    Bitboard piece_occupied = board.get_piece(color, piece);
    Bitboard friendly = board.get_color_occupied(color);
    Bitboard enemy = board.get_color_occupied(board.flip_color(color));

    while (piece_occupied.get_bitboard()) {
      SQUARE origin = static_cast<SQUARE>(piece_occupied.get_least_significant_bit());
      Bitboard attack = move_table.get_moves(piece, origin);
      attack.AND(friendly.GET_NOT());

      while (attack.get_bitboard()) {
        SQUARE target = static_cast<SQUARE>(attack.get_least_significant_bit());
        moves.push_back(Move(origin, target, enemy.is_occupied(target) ? FLAG::CAPTURE : FLAG::NONE));
        attack.clear_bit(target);
      }
      piece_occupied.clear_bit(origin);
    }
    return moves;
  }

public:
  // todo: correctly classify pseudo legal move
  std::vector<Move> generate_legal_moves(const Board &board) {
    std::vector<Move> legal_moves;
    return legal_moves;
  }

  std::vector<Move> generate_knight_moves(Board &board) {
    return generate_non_sliding_moves(board, PIECE::KNIGHT);
  }

  std::vector<Move> generate_bishop_moves(Board &board) {
    return generate_sliding_moves(board, PIECE::BISHOP);
  }

  std::vector<Move> generate_rook_moves(Board &board) {
    return generate_sliding_moves(board, PIECE::ROOK);
  }

  std::vector<Move> generate_queen_moves(Board &board) {
    return generate_sliding_moves(board, PIECE::QUEEN);
  }

  std::vector<Move> generate_king_moves(Board &board) {
    return generate_non_sliding_moves(board, PIECE::KING);
  }
};
