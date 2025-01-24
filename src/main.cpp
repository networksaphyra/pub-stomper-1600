#include <iostream>
#include <cstdint>
#include <map>
#include "../include/board.h"
#include "../include/evaluate.h"
#include "../include/move.h"
#include "../include/constants.h"

int main() {
  Board board;
  Evaluate eval;

  board.make_move(Move(B1, C3));
  board.make_move(Move(B8, C6));
  board.make_move(Move(G1, F3));

  std::cout << board.get_piece_at(B8).second << "\n";
  std::cout << board.get_piece_at(C6).second << "\n";
  std::cout << eval.evaluate(board) << "\n";
}
