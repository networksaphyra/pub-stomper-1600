#pragma once
#include <cstdint>
#include "constants.cpp"

class Bitboard {
private:
  uint64_t bitboard;

public:
  Bitboard(uint64_t bb = 0ULL): bitboard(bb) {}

  bool is_occupied(int square) { return GET_AND(1ULL << square); }
  uint64_t get_bitboard() { return bitboard; }
  int get_least_significant_bit() {
    for (int i=0; i<64; ++i) {
      if (is_occupied(i)) return i;
    }
    return -1;
  }

  void set_bitboard(uint64_t bb) { bitboard = bb; }
  void set_bitboard(Bitboard bb) { bitboard = bb.get_bitboard(); }
  void set_bit(int bit) { bitboard |= (1ULL << bit); }
  void clear_bit(int bit) { bitboard &= ~(1ULL << bit); }

  void OR(uint64_t other) { bitboard |= other; }
  void OR(Bitboard other) { bitboard |= other.get_bitboard(); }
  void AND(uint64_t other) { bitboard &= other; }
  void AND(Bitboard other) { bitboard &= other.get_bitboard(); }
  void XOR(uint64_t other) { bitboard ^= other; }
  void XOR(Bitboard other) { bitboard ^= other.get_bitboard(); }
  void NOT() { bitboard=~bitboard; }

  uint64_t GET_OR(uint64_t other) { return bitboard | other; }
  uint64_t GET_OR(Bitboard other) { return bitboard | other.get_bitboard(); }
  uint64_t GET_AND(uint64_t other) { return bitboard & other; }
  uint64_t GET_AND(Bitboard other) { return bitboard & other.get_bitboard(); }
  uint64_t GET_XOR(uint64_t other) { return bitboard ^ other; }
  uint64_t GET_XOR(Bitboard other) { return bitboard ^ other.get_bitboard(); }
  uint64_t GET_NOT() { return ~bitboard; }
};
