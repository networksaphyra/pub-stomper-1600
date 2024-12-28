#include "bitboard.cpp"
#include "constants.cpp"
#include <cstdint>
#include <array>
#include <vector>

// Piece Centric Approach: Keeps a list of all pieces on the board, with associated info about what squares they occupy
// We will need one bitboard for each piece-type and color

/*
Board state MUST contain the following:
1) The position of all pieces on the board
2) Current player turn
3) Number of reversible moves played so far
4) State of castling king side or queen side for both colors
5) Move history for three fold repetition
*/

class Board {
private:
  Bitboard occupied;
  std::array<std::array<Bitboard, 6>, 2> pieces;

  COLOR player_color, engine_color;
  COLOR turn;

  int reversible_moves = 0;
  std::vector<int> move_list;

  // Bit 1: White king side castle, Bit 2: White queen side castle, Bit 3: Black king side castle, Bit 4: Black queen side castle
  uint8_t castling = 0b00001111;

  void update_occupied() {
    uint64_t new_occupied = 0ULL;
    for (int color=0; color<2; ++color) {
      for (int piece_type=0; piece_type<6; ++piece_type) {
        new_occupied |= pieces[color][piece_type].get_bitboard();
      }
    }
    occupied.set_bitboard(new_occupied);
  }

  void setup_initial_position() {
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
  Board() {
    turn = WHITE;
    set_player_color(WHITE);
    setup_initial_position();
  }

  std::array<std::array<Bitboard, 6>, 2> get_pieces() { return pieces; }
  COLOR get_turn() { return turn; }

  void next_turn() { turn = (turn == WHITE ? BLACK: WHITE); }
  void set_reversible_moves(int moves) { reversible_moves = moves; }
  // this might bite me later, but quick, dirty way to invalidate castling without proper checks.
  void invalidate_castling(COLOR color, CASTLE_TYPE castle_type) { castling &= ~(1 << (castle_type + (color == BLACK ? 2: 0))); }

  void set_player_color(COLOR color) {
    player_color = color;
    engine_color = (color == WHITE ? BLACK: WHITE);
  }
};
