#include <chess/bitboard.hpp>
#include <stdexcept>
#include <iostream>

void print_bitboard(Bitboard b)
{
    for (int rank = 7; rank >= 0; --rank)
    {
        std::cout <<  rank + 1 << "   ";
        for (int file = 0; file < 8; ++file)
        {
            int square = rank * 8 + file;
            
            // Bitshift 1 left in order to check if there is a 1 in the square 
            std::cout << (b & (1ULL << square) ? 1 : 0) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "    A B C D E F G H" << std::endl;
    std::cout << std::endl;
}

// Least significant bit
int calcLSB(const Bitboard &b)
{
    return __builtin_ctzll(b);
}

void toggleBit(Bitboard &b, Pos square) {
    b ^= 1ULL << square;
}

void clearBit(Bitboard &b, Pos square) {
    b &= ~(1ULL << square);
}

int count_bits(Bitboard number) {
    int count = 0;
    while (number) {
        number &= (number - 1);
        count++;
    }
    return count;
}

U64 mask_between(int square1, int square2) {
    
    U64 mask = 0ULL;
    int direction = 0;

    const int& lower = std::min(square1, square2);
    const int& upper = std::max(square1, square2);

    // Determine direction
    if (square1 / 8 == square2 / 8)      direction = 1;  // Same rank
    else if (square1 % 8 == square2 % 8) direction = 8;  // Same file
    else if (abs(square1 - square2) % 9 == 0) direction = 9;  // Diagonal
    else if (abs(square1 - square2) % 7 == 0) direction = 7;  // Diagonal
    else throw std::invalid_argument("Invalid squares");
    
    for (int square = lower + direction; square != upper; square += direction) {
        mask |= 1ULL << square;
    }

    return mask;

}
