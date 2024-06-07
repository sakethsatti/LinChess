#include <iostream>
#include <bitset>
#include "ChessGame/board.hpp"

int main() {
    Board board = Board("qrb5/rk1p1K2/p2P4/Pp6/1N2n3/6p1/5nB1/6b1 w - - 0 1");
    board.print_position();
    return 0;
}