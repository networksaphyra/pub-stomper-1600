#include "bitboard.cpp"
#include "constants.cpp"
#include <cstdint>

// Piece Centric Approach: Keeps a list of all pieces on the board, with associated info about what squares they occupy
// We will need one bitboard for each piece-type and color

class Board {
private:
  Bitboard occupied;
  Bitboard pieces[2][6];

public:
  void update_occupied() {
    int64_t new_occupied = 0ULL;
    for (int color=0; color<2; ++color) {
      for (int piece_type=0; piece_type<6; ++piece_type) {
        new_occupied |= pieces[color][piece_type].get_bitboard();
      }
    }
    occupied.set_bitboard(new_occupied);
  }

};
