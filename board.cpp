// Contains game state, where pieces are, and other essentials
#include <cstdint>
#include <array>
#include "piece.cpp"

class Board {
private:
  // Bitset for the chessboard
  uint64_t all_pieces;
  // Bitset for each piece
  std::array<std::array<uint64_t, 6>, 2> pieces;

public:
  Board(): all_pieces(0ULL) {
    for (int i=0; i<6; ++i) {
      pieces[0][i]=0ULL;
      pieces[0][i]=0ULL;
    }
    setup_initial_position();
  }

private:
  void setup_initial_position() {
    pieces[PieceColor::WHITE][PieceType::PAWN] = 0x00FF000000000000ULL;
    pieces[PieceColor::BLACK][PieceType::PAWN] = 0x000000000000FF00ULL;

    pieces[PieceColor::WHITE][PieceType::KNIGHT] = (0ULL | (1ULL << 1)) | ((0ULL) | (1ULL << 6));
    pieces[PieceColor::BLACK][PieceType::KNIGHT] = (0ULL | (1ULL << 57)) | ((0ULL) | (1ULL << 62));

    pieces[PieceColor::WHITE][PieceType::BISHOP] = (0ULL | (1ULL << 2)) | ((0ULL) | (1ULL << 5));
    pieces[PieceColor::BLACK][PieceType::BISHOP] = (0ULL | (1ULL << 58)) | ((0ULL) | (1ULL << 61));

    pieces[PieceColor::WHITE][PieceType::ROOK] = (0ULL | (1ULL << 0)) | ((0ULL) | (1ULL << 7));
    pieces[PieceColor::BLACK][PieceType::ROOK] = (0ULL | (1ULL << 56)) | ((0ULL) | (1ULL << 63));

    pieces[PieceColor::WHITE][PieceType::QUEEN] = (0ULL | (1ULL << 3));
    pieces[PieceColor::BLACK][PieceType::QUEEN] = (0ULL | (1ULL << 59));

    pieces[PieceColor::WHITE][PieceType::KING] = (0ULL | (1ULL << 4));
    pieces[PieceColor::BLACK][PieceType::KING] = (0ULL | (1ULL << 60));

    update_all_pieces();
  }

  void update_all_pieces() {
    all_pieces = 0ULL;
    for (int i=0; i<6; ++i) {
      all_pieces |= pieces[PieceColor::WHITE][i] | pieces[PieceColor::BLACK][i];
    }
  }
};
