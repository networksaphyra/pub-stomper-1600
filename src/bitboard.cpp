#include <cstdint>

class Bitboard {
private:
  int64_t bitboard;

public:
  Bitboard(): bitboard(0ULL) {}

  bool is_occupied(int bit) { return bitboard & (1 << bit); }
  int64_t get_bitboard() { return bitboard; }

  void set_bitboard(int64_t bb) { bitboard = bb; }
  void set_bit(int bit) { bitboard |= (1 << bit); }
  void clear_bit(int bit) { bitboard &= ~(1 << bit); }
};
