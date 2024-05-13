#include <iostream>
#include <array>
#include "ChessGame/board.hpp"

int main() {    
    Board board;
    board.print_board();
    board.print_attack_bitboards();        

    return 0;
}