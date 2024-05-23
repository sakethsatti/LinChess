#include "bitboard.hpp"
#include <iostream>

void print_bitboard(Bitboard b)
{
    for (int i = 0; i < 64; i++)
    {
        if (i % 8 == 0)
        {
            std::cout << std::endl;
        }

        // Bit shift 1 left i times, then bitwise AND with the board.
        std::cout << (b  & (1ULL << i));
    }
    std::cout << std::endl;
}