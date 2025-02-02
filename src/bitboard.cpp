#include "../include/bitboard.h"

Bitboard::Bitboard(uint64_t bb) : bitboard(bb) {}

bool Bitboard::is_occupied(int square) {
  return (bitboard & (1ULL << square)) != 0;
}

uint64_t Bitboard::get_bitboard() {
  return bitboard;
}

int Bitboard::get_least_significant_bit() {
  if (bitboard == 0) return -1;
  return __builtin_ctzll(bitboard);
}

int Bitboard::pop_least_significant_bit() {
  if (bitboard == 0) return -1;
  int lsb = get_least_significant_bit();
  bitboard &= (bitboard - 1);
  return lsb;
}

int Bitboard::count_bits() {
  return __builtin_popcountll(bitboard);
}

void Bitboard::set_bitboard(uint64_t bb) {
  bitboard = bb;
}

void Bitboard::set_bitboard(Bitboard bb) {
  bitboard = bb.get_bitboard();
}

void Bitboard::set_bit(int bit) {
  bitboard |= (1ULL << bit);
}

void Bitboard::clear_bit(int bit) {
  bitboard &= ~(1ULL << bit);
}

void Bitboard::clear_all() {
  bitboard = 0ULL;
}

void Bitboard::OR(uint64_t other) {
  bitboard |= other;
}

void Bitboard::OR(Bitboard other) {
  bitboard |= other.get_bitboard();
}

void Bitboard::AND(uint64_t other) {
  bitboard &= other;
}

void Bitboard::AND(Bitboard other) {
  bitboard &= other.get_bitboard();
}

void Bitboard::XOR(uint64_t other) {
  bitboard ^= other;
}

void Bitboard::XOR(Bitboard other) {
  bitboard ^= other.get_bitboard();
}

void Bitboard::NOT() {
  bitboard = ~bitboard;
}

uint64_t Bitboard::GET_OR(uint64_t other) {
  return bitboard | other;
}

uint64_t Bitboard::GET_OR(Bitboard other) {
  return bitboard | other.get_bitboard();
}

uint64_t Bitboard::GET_AND(uint64_t other) {
  return bitboard & other;
}

uint64_t Bitboard::GET_AND(Bitboard other) {
  return bitboard & other.get_bitboard();
}

uint64_t Bitboard::GET_XOR(uint64_t other) {
  return bitboard ^ other;
}

uint64_t Bitboard::GET_XOR(Bitboard other) {
  return bitboard ^ other.get_bitboard();
}

uint64_t Bitboard::GET_NOT() {
  return ~bitboard;
}
