#include <iostream>
#include <array>
#include <string>
#include "ChessGame/board.hpp"

int main() {    
    Board board;
    board.print_board();
    std::cout << "Welcome to ChessGame!" << std::endl;

    std::string move;

    std::cout << "Enter a move: ";
    std::cin >> move;
    std::cout << std::endl;

    int waste = std::stoi(move);
    int old_file = waste / 1000;
    waste %= 1000;
    int old_rank = waste / 100;
    waste %= 100;
    int new_file = waste / 10;
    waste %= 10;
    int new_rank = waste;
    std::cout << old_file << " " << old_rank << " " << new_file << " " << new_rank << std::endl;
    
    board.move_piece(old_file, old_rank, new_file, new_rank);

    return 0;
}