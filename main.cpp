#include <iostream>
#include "ChessGame/board.hpp"


int main(int argc, char **argv) {
  Board board = Board(argv[1]);

  std::cout << board.genLegalMoves().size() << std::endl;
}

