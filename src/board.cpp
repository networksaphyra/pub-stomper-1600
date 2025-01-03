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
  uint8_t castling = 0b1111;

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
    pieces[COLOR::WHITE][PIECE::PAWN].set_bitboard(SECOND_RANK);
    pieces[COLOR::BLACK][PIECE::PAWN].set_bitboard(SEVENTH_RANK);

    pieces[COLOR::WHITE][PIECE::KNIGHT].set_bit(SQUARE::B1);
    pieces[COLOR::WHITE][PIECE::KNIGHT].set_bit(SQUARE::G1);
    pieces[COLOR::BLACK][PIECE::KNIGHT].set_bit(SQUARE::B8);
    pieces[COLOR::BLACK][PIECE::KNIGHT].set_bit(SQUARE::G8);

    pieces[COLOR::WHITE][PIECE::BISHOP].set_bit(SQUARE::C1);
    pieces[COLOR::WHITE][PIECE::BISHOP].set_bit(SQUARE::F1);
    pieces[COLOR::BLACK][PIECE::BISHOP].set_bit(SQUARE::C8);
    pieces[COLOR::BLACK][PIECE::BISHOP].set_bit(SQUARE::F8);

    pieces[COLOR::WHITE][PIECE::ROOK].set_bit(SQUARE::A1);
    pieces[COLOR::WHITE][PIECE::ROOK].set_bit(SQUARE::H1);
    pieces[COLOR::BLACK][PIECE::ROOK].set_bit(SQUARE::A8);
    pieces[COLOR::BLACK][PIECE::ROOK].set_bit(SQUARE::H8);

    pieces[COLOR::WHITE][PIECE::QUEEN].set_bit(SQUARE::D1);
    pieces[COLOR::BLACK][PIECE::QUEEN].set_bit(SQUARE::D8);

    pieces[COLOR::WHITE][PIECE::KING].set_bit(SQUARE::E1);
    pieces[COLOR::BLACK][PIECE::KING].set_bit(SQUARE::E8);
  }

public:
  Board() {
    turn = WHITE;
    set_player_color(WHITE);
    setup_initial_position();
  }

  std::array<std::array<Bitboard, 6>, 2> get_pieces() { return pieces; }
  Bitboard get_piece(COLOR color, PIECE piece_type) { return pieces[color][piece_type]; }
  COLOR get_turn() { return turn; }
  bool can_castle(COLOR color, CASTLE castle_type) { return (castling & (1 << (castle_type + (color == BLACK ? 2: 0)))) != 0; }

  void next_turn() { turn = (turn == WHITE ? BLACK: WHITE); }
  void set_reversible_moves(int moves) { reversible_moves = moves; }
  void invalidate_castling(COLOR color, CASTLE castle_type) { castling &= ~(1 << (castle_type + (color == BLACK ? 2: 0))); }

  void set_player_color(COLOR color) {
    player_color = color;
    engine_color = (color == WHITE ? BLACK: WHITE);
  }
};
