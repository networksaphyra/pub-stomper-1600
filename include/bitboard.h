#pragma once
#include <cstdint>

class Bitboard {
private:
  uint64_t bitboard;

public:
  Bitboard(uint64_t bb = 0ULL);

  bool is_occupied(int square);

  uint64_t get_bitboard();
  int get_least_significant_bit();

  void set_bitboard(uint64_t bb);
  void set_bitboard(Bitboard bb);
  void set_bit(int bit);
  void clear_bit(int bit);
  void clear_all();

  void OR(uint64_t other);
  void OR(Bitboard other);
  void AND(uint64_t other);
  void AND(Bitboard other);
  void XOR(uint64_t other);
  void XOR(Bitboard other);
  void NOT();

  uint64_t GET_OR(uint64_t other);
  uint64_t GET_OR(Bitboard other);
  uint64_t GET_AND(uint64_t other);
  uint64_t GET_AND(Bitboard other);
  uint64_t GET_XOR(uint64_t other);
  uint64_t GET_XOR(Bitboard other);
  uint64_t GET_NOT();
};
