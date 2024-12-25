#include <cstdint>
#include "piece.cpp"

class Board {
public:
  // let pieces[null] refer to captured piece
  PieceType pieces[10];
};
