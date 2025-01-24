#include "../include/evaluate.h"

const std::array<int, 6> PIECE_VALUES {
  100,
  320,
  330,
  500,
  900,
  20000
};

int PIECE_SQUARE_TABLES[2][6][64] = {
  {
  // Note: All these tables have A1 as bottom left, so we're gonna have to do some math when we use them.
  // Order of tables: PAWN, KNIGHT, BISHOP, ROOK, QUEEN
  // Also, mirror these values for BLACK
    {
      0,  0,  0,  0,  0,  0,  0,  0,
      50, 50, 50, 50, 50, 50, 50, 50,
      10, 10, 20, 30, 30, 20, 10, 10,
      5,  5, 10, 25, 25, 10,  5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5, -5,-10,  0,  0,-10, -5,  5,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0
    },
    {
      -50,-40,-30,-30,-30,-30,-40,-50,
      -40,-20,  0,  0,  0,  0,-20,-40,
      -30,  0, 10, 15, 15, 10,  0,-30,
      -30,  5, 15, 20, 20, 15,  5,-30,
      -30,  0, 15, 20, 20, 15,  0,-30,
      -30,  5, 10, 15, 15, 10,  5,-30,
      -40,-20,  0,  5,  5,  0,-20,-40,
      -50,-40,-30,-30,-30,-30,-40,-50,
    },
    {
      -20,-10,-10,-10,-10,-10,-10,-20,
      -10,  0,  0,  0,  0,  0,  0,-10,
      -10,  0,  5, 10, 10,  5,  0,-10,
      -10,  5,  5, 10, 10,  5,  5,-10,
      -10,  0, 10, 10, 10, 10,  0,-10,
      -10, 10, 10, 10, 10, 10, 10,-10,
      -10,  5,  0,  0,  0,  0,  5,-10,
      -20,-10,-10,-10,-10,-10,-10,-20,
    },
    {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0
    },
    {
      -20,-10,-10, -5, -5,-10,-10,-20,
      -10,  0,  0,  0,  0,  0,  0,-10,
      -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
      -10,  5,  5,  5,  5,  5,  0,-10,
      -10,  0,  5,  0,  0,  0,  0,-10,
      -20,-10,-10, -5, -5,-10,-10,-20
    },
    {
      -30,-40,-40,-50,-50,-40,-40,-30,
      -30,-40,-40,-50,-50,-40,-40,-30,
      -30,-40,-40,-50,-50,-40,-40,-30,
      -30,-40,-40,-50,-50,-40,-40,-30,
      -20,-30,-30,-40,-40,-30,-30,-20,
      -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    }
  },
  {}
};

Evaluation::Evaluation() {
  for (PIECE piece = PIECE::PAWN; piece <= PIECE::KING; piece = static_cast<PIECE>(piece + 1)) {
    for (SQUARE square = A1; square <= H8; square = static_cast<SQUARE>(square + 1)) {
      PIECE_SQUARE_TABLES[COLOR::BLACK][piece][square] = PIECE_SQUARE_TABLES[COLOR::WHITE][piece][63 - square];
    }
  }
}

int Evaluation::evaluate(Board& board) {
  int score = 0;
  COLOR perspective = board.get_color();
  COLOR opposite = board.flip_color(perspective);

  for (int color = COLOR::WHITE; color <= COLOR::BLACK; ++color) {
    int color_multiplier = (color == perspective) ? 1 : -1;
    for (int piece = PIECE::PAWN; piece <= PIECE::KING; ++piece) {
      Bitboard piece_bb = board.get_piece(static_cast<COLOR>(color), static_cast<PIECE>(piece));
      while (piece_bb.get_bitboard()) {
        SQUARE square = static_cast<SQUARE>(piece_bb.pop_least_significant_bit());

        score += color_multiplier * PIECE_VALUES[piece];

        score += color_multiplier * PIECE_SQUARE_TABLES[color][piece][
          color == COLOR::WHITE ? square : (63 - square)
        ];
      }
    }
  }

  return score;
}
