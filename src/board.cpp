#include "bitboard.cpp"
#include "constants.cpp"
#include <cstdint>
#include <array>

// Piece Centric Approach: Keeps a list of all pieces on the board, with associated info about what squares they occupy
// We will need one bitboard for each piece-type and color

class Board {
private:
  Bitboard occupied;
  std::array<std::array<Bitboard, 6>, 2> pieces;

  void update_occupied() {
    uint64_t new_occupied = 0ULL;
    for (int color=0; color<2; ++color) {
      for (int piece_type=0; piece_type<6; ++piece_type) {
        new_occupied |= pieces[color][piece_type].get_bitboard();
      }
    }
    occupied.set_bitboard(new_occupied);
  }

  void setup_initial_positions() {
    pieces[COLOR::WHITE][PIECE_TYPE::PAWN].set_bitboard(0x000000000000FF00ULL);
    pieces[COLOR::BLACK][PIECE_TYPE::PAWN].set_bitboard(0x00FF000000000000ULL);

    pieces[COLOR::WHITE][PIECE_TYPE::KNIGHT].set_bit(1);
    pieces[COLOR::WHITE][PIECE_TYPE::KNIGHT].set_bit(6);
    pieces[COLOR::BLACK][PIECE_TYPE::KNIGHT].set_bit(57);
    pieces[COLOR::BLACK][PIECE_TYPE::KNIGHT].set_bit(62);

    pieces[COLOR::WHITE][PIECE_TYPE::BISHOP].set_bit(2);
    pieces[COLOR::WHITE][PIECE_TYPE::BISHOP].set_bit(5);
    pieces[COLOR::BLACK][PIECE_TYPE::BISHOP].set_bit(58);
    pieces[COLOR::BLACK][PIECE_TYPE::BISHOP].set_bit(61);

    pieces[COLOR::WHITE][PIECE_TYPE::ROOK].set_bit(0);
    pieces[COLOR::WHITE][PIECE_TYPE::ROOK].set_bit(7);
    pieces[COLOR::BLACK][PIECE_TYPE::ROOK].set_bit(56);
    pieces[COLOR::BLACK][PIECE_TYPE::ROOK].set_bit(63);

    pieces[COLOR::WHITE][PIECE_TYPE::QUEEN].set_bit(3);
    pieces[COLOR::BLACK][PIECE_TYPE::QUEEN].set_bit(59);

    pieces[COLOR::WHITE][PIECE_TYPE::KING].set_bit(4);
    pieces[COLOR::BLACK][PIECE_TYPE::KING].set_bit(60);
  }

public:
  Board() { setup_initial_positions(); }

  std::array<std::array<Bitboard, 6>, 2> get_pieces() {
    return pieces;
  }
};
