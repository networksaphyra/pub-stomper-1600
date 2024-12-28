#include "board.cpp"

struct Move {
  COLOR color;
  PIECE_TYPE piece_type;
  int from;
  int to;
  PIECE_TYPE captured_piece;
};

class MoveGen {
  Move generate_moves(Board& board) {

  }
};
