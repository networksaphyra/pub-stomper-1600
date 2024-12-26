#include <cstdint>

class Bitboard {
private:
  uint64_t bitboard;

public:
  Bitboard(): bitboard(0ULL) {}

  bool is_occupied(int bit) { return (bitboard & (1ULL << bit)) != 0; }
  uint64_t get_bitboard() { return bitboard; }

  void set_bitboard(uint64_t bb) { bitboard = bb; }
  void set_bit(int bit) { bitboard |= (1ULL << bit); }
  void clear_bit(int bit) { bitboard &= ~(1ULL << bit); }
};
