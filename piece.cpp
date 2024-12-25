#include <string>
#include <cstdint>
#include <array>

enum class Color {White, Black};
enum class PieceType {King, Queen, Rook, Bishop, Knight, Pawn};

class Piece {
private:
  PieceType type;
  Color color;
  uint64_t bitboard;

public:
  Piece(PieceType type, Color color) : type(type), color(color), bitboard(0ULL) {}

  // Getters
  u_int64_t get_bitboard() { return bitboard; }
  Color get_color() { return color; }
  PieceType get_type() { return type; }

  // Setters
  void set_bit(int square) { bitboard |= (1ULL << square); }
  void delete_bit(int square) { bitboard &= ~(1ULL << square); }
  bool is_occupied(int square) { return bitboard & (1ULL << square); }
};
