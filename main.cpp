#include <iostream>
#include <array>
#include <string>
#include "ChessGame/bitboard.hpp"

int main() {    
    print_bitboard(FILE_A);
    print_bitboard(FILE_H);
    print_bitboard(RANK_1);
    print_bitboard(RANK_8);

    return 0;
}