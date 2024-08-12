#include <chess/board.hpp>
/*
  
*/


int main(int argc, char **argv) {
  Board board(argv[1]);
  perftRunner(board, atoi(argv[2]), atoi(argv[2]));


}

