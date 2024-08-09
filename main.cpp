#include <iostream>
#include "ChessGame/board.hpp"

// 
int main(int argc, char **argv) {
  Board board = Board(argv[1]); 
  
  std::cout << perftRunner(board, atoi(argv[2]), atoi(argv[2])) << std::endl;
}

