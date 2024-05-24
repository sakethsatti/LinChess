#include <iostream>
#include <array>
#include <string>
#include "ChessGame/bitboard.hpp"

int main() {    
    print_bitboard(king_attacks(e7));

    return 0;
}