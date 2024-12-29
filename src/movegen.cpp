#include "board.cpp"

struct Move {
  COLOR color;
  PIECE piece_type;
  int from;
  int to;
  PIECE captured_piece;
};

class MoveGen {
  Move generate_moves(Board& board) {

  }
};
