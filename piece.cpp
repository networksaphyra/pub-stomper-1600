#include <cstdint>
#include <iostream>

enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
enum PieceColor { WHITE, BLACK };

class Piece {
private:
  uint64_t bitboard;
  PieceColor color;
  PieceType piece_name;


public:
  Piece(uint64_t bitboard, PieceType piece_name, PieceColor color):
  bitboard(bitboard), color(color), piece_name(piece_name) {};

  // Getters
  bool is_occupied(int bit) { return (bitboard & (1ULL << bit)) != 0; }
  PieceColor get_color() { return color; }
  PieceType get_name() { return piece_name; }

  // Setters
  void set_bit(int bit) { bitboard |= (1ULL << bit); }
  void clear_bit(int bit) { bitboard &= (1ULL << bit); }
};
