#pragma once

#include "../include/movegen.h"

bool MoveGenerator::is_square_attacked(Board& board, SQUARE square, COLOR attacker_color) {
  COLOR original_turn = board.get_color();
  board.set_turn(attacker_color);

  auto is_attacking = [square](Move move) {
    return move.get_target() == square;
  };

  for (PIECE piece = PIECE::KNIGHT; piece <= PIECE::KING; piece = static_cast<PIECE>(piece + 1)) {
    std::vector<Move> piece_moves = generate_piece_moves(board, piece);
    if (std::any_of(piece_moves.begin(), piece_moves.end(), is_attacking)) {
      return true;
    }
  }

  board.set_turn(original_turn);

  Bitboard pawns = board.get_piece(attacker_color, PIECE::PAWN);
  int direction = (attacker_color == COLOR::WHITE) ? 1 : -1;
  BOARD_FILE file = get_file(square);

  if (file > FILE_A) {
    SQUARE attack_source = static_cast<SQUARE>(square - direction * 8 - 1);
    if (attack_source >= 0 && attack_source < 64 && pawns.is_occupied(attack_source)) {
      return true;
    }
  }
  if (file < FILE_H) {
    SQUARE attack_source = static_cast<SQUARE>(square - direction * 8 + 1);
    if (attack_source >= 0 && attack_source < 64 && pawns.is_occupied(attack_source)) {
      return true;
    }
  }

  return false;
}

bool MoveGenerator::is_check(Board& board, COLOR color) {
  SQUARE king_square = static_cast<SQUARE>(
    board.get_piece(color, PIECE::KING).get_least_significant_bit()
  );
  return is_square_attacked(board, king_square, board.flip_color(color));
}

bool MoveGenerator::is_move_legal(Board& board, const Move& move) {
  Board test_board = board;
  test_board.make_move(move);
  COLOR moving_color = board.get_color();
  return !is_check(test_board, moving_color);
}

std::vector<Move> MoveGenerator::generate_castling_moves(Board& board) {
  std::vector<Move> moves;
  COLOR color = board.get_color();
  BOARD_RANK rank = (color == COLOR::WHITE) ? RANK_1 : RANK_8;

  if (is_check(board, color)) {
    return moves;
  }

  if (board.can_castle(color, CASTLE::KING_SIDE)) {
    if (!board.get_occupied().is_occupied(make_square(FILE_F, rank)) &&
        !board.get_occupied().is_occupied(make_square(FILE_G, rank))) {

      bool safe = true;
      for (auto file : {FILE_E, FILE_F, FILE_G}) {
        if (is_square_attacked(board, make_square(file, rank), board.flip_color(color))) {
          safe = false;
          break;
        }
      }

      if (safe) {
        moves.emplace_back(
          make_square(FILE_E, rank),
          make_square(FILE_G, rank),
          FLAG::CASTLING | FLAG::KING_CASTLE
        );
      }
    }
  }

  if (board.can_castle(color, CASTLE::QUEEN_SIDE)) {
    if (!board.get_occupied().is_occupied(make_square(FILE_D, rank)) &&
        !board.get_occupied().is_occupied(make_square(FILE_C, rank)) &&
        !board.get_occupied().is_occupied(make_square(FILE_B, rank))) {

      bool safe = true;
      for (auto file : {FILE_E, FILE_D, FILE_C}) {
        if (is_square_attacked(board, make_square(file, rank), board.flip_color(color))) {
          safe = false;
          break;
        }
      }

      if (safe) {
        moves.emplace_back(
          make_square(FILE_E, rank),
          make_square(FILE_C, rank),
          FLAG::CASTLING | FLAG::QUEEN_CASTLE
        );
      }
    }
  }

  return moves;
}

std::vector<Move> MoveGenerator::generate_sliding_moves(Board &board, PIECE piece) {
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
      moves.push_back(
        Move(origin, target, enemy.is_occupied(target) ? FLAG::CAPTURE : FLAG::NONE)
      );
      moves_bb.clear_bit(target);
    }
    pieces.clear_bit(origin);
  }
  return moves;
}

std::vector<Move> MoveGenerator::generate_non_sliding_moves(Board &board, PIECE piece) {
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
      moves.push_back(
        Move(origin, target, enemy.is_occupied(target) ? FLAG::CAPTURE : FLAG::NONE)
      );
      attack.clear_bit(target);
    }
    piece_occupied.clear_bit(origin);
  }
  return moves;
}

std::vector<Move> MoveGenerator::generate_pawn_moves(Board &board) {
  std::vector<Move> pawn_moves;
  COLOR color = board.get_color();
  Bitboard pawns = board.get_piece(color, PIECE::PAWN);
  Bitboard occupied = board.get_occupied();
  Bitboard enemy = board.get_color_occupied(board.flip_color(color));

  int direction = (color == COLOR::WHITE) ? 1 : -1;
  BOARD_RANK starting_rank = (color == COLOR::WHITE) ? RANK_2 : RANK_7;
  BOARD_RANK promotion_rank = (color == COLOR::WHITE) ? RANK_8 : RANK_1;
  BOARD_RANK en_passant_rank = (color == COLOR::WHITE) ? RANK_5 : RANK_4;

  while (pawns.get_bitboard()) {
    SQUARE origin = static_cast<SQUARE>(pawns.pop_least_significant_bit());
    BOARD_RANK origin_rank = get_rank(origin);
    BOARD_FILE origin_file = get_file(origin);

    SQUARE single_push = static_cast<SQUARE>(origin + (direction * 8));
    if (single_push >= 0 && single_push < 64 && !occupied.is_occupied(single_push)) {
      if (get_rank(single_push) == promotion_rank) {

        pawn_moves.emplace_back(origin, single_push, FLAG::PAWN_PROMOTION | FLAG::PROMOTION_QUEEN);
        pawn_moves.emplace_back(origin, single_push, FLAG::PAWN_PROMOTION | FLAG::PROMOTION_ROOK);
        pawn_moves.emplace_back(origin, single_push, FLAG::PAWN_PROMOTION | FLAG::PROMOTION_BISHOP);
        pawn_moves.emplace_back(origin, single_push, FLAG::PAWN_PROMOTION | FLAG::PROMOTION_KNIGHT);
      } else {
        pawn_moves.emplace_back(origin, single_push, FLAG::NONE);

        if (origin_rank == starting_rank) {
          SQUARE double_push = static_cast<SQUARE>(origin + (direction * 16));
          if (!occupied.is_occupied(double_push)) {
            pawn_moves.emplace_back(origin, double_push, FLAG::DOUBLE_PAWN_PUSH);
          }
        }
      }
    }

    for (int delta_file : {-1, 1}) {
      if (origin_file + delta_file >= FILE_A && origin_file + delta_file <= FILE_H) {
        SQUARE capture_square = static_cast<SQUARE>(origin + (direction * 8) + delta_file);

        if (capture_square >= 0 && capture_square < 64) {
          if (enemy.is_occupied(capture_square)) {
            if (get_rank(capture_square) == promotion_rank) {
              pawn_moves.emplace_back(origin, capture_square,
                FLAG::PAWN_PROMOTION | FLAG::CAPTURE | FLAG::PROMOTION_QUEEN);
              pawn_moves.emplace_back(origin, capture_square,
                FLAG::PAWN_PROMOTION | FLAG::CAPTURE | FLAG::PROMOTION_ROOK);
              pawn_moves.emplace_back(origin, capture_square,
                FLAG::PAWN_PROMOTION | FLAG::CAPTURE | FLAG::PROMOTION_BISHOP);
              pawn_moves.emplace_back(origin, capture_square,
                FLAG::PAWN_PROMOTION | FLAG::CAPTURE | FLAG::PROMOTION_KNIGHT);
            } else {
              pawn_moves.emplace_back(origin, capture_square, FLAG::CAPTURE);
            }
          }

          if (origin_rank == en_passant_rank) {
            SQUARE en_passant_square = board.get_en_passant_square();
            if (en_passant_square == capture_square) {
              pawn_moves.emplace_back(origin, capture_square, FLAG::EN_PASSANT | FLAG::CAPTURE);
            }
          }
        }
      }
    }
  }

  return pawn_moves;
}

std::vector<Move> MoveGenerator::generate_legal_moves(Board& board) {
  std::vector<Move> legal_moves;
  std::vector<Move> candidate_moves;

  for (int piece = PIECE::PAWN; piece <= PIECE::KING; ++piece) {
    std::vector<Move> piece_moves = generate_piece_moves(board, static_cast<PIECE>(piece));
    candidate_moves.insert(candidate_moves.end(), piece_moves.begin(), piece_moves.end());
  }

  for (const auto& move : candidate_moves) {
    if (is_move_legal(board, move)) {
      legal_moves.push_back(move);
    }
  }

  return legal_moves;
}

std::vector<Move> MoveGenerator::generate_piece_moves(Board &board, PIECE piece) {
  std::vector<Move> piece_moves;

  if (piece == PIECE::PAWN) {
    piece_moves = generate_pawn_moves(board);
  } else if (piece == PIECE::KNIGHT || piece == PIECE::KING) {
    piece_moves = generate_non_sliding_moves(board, piece);
  } else {
    piece_moves = generate_sliding_moves(board, piece);
  }

  return piece_moves;
}
