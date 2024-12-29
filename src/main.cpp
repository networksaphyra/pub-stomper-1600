#include <iostream>
#include <cstdint>
#include "board.cpp"

int main() {
  Board board;
  auto pieces = board.get_pieces();
  for (int i=0; i<2; ++i) {
    for (int j=0; j<6; ++j) {
      std::cout << "Piece Color: " << i << " | " << "Piece Name: " << j << "\n";
      std::cout << pieces[i][j].get_bitboard() << "\n";
    }
  }
}
