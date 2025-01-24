#include <iostream>
#include <cstdint>
#include "../include/board.h"
#include "../include/evaluate.h"
#include "../include/move.h"
#include "../include/constants.h"

int main() {
  Board board;
  Evaluate eval;
  SQUARE origin = B1;
  SQUARE target = C3;
  Move move(origin, target);
  board.make_move(move);
  std::cout << eval.evaluate(board) << "\n";
}
