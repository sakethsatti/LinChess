#include <iostream>
#include <bitset>
#include "ChessGame/bitboard.hpp"

int main() {
    pos sq = b1;
    BlockerTable blockers = calcBishopBlockers(sq);


    int key = (blockers[12] * bishop_magics[sq]) >> (64 - bishop_relevant_bits[sq]);
    key += 64;

    print_bitboard(blockers[12]);
    print_bitboard(BISHOP_TABLE[key]);

    // auto magics = findAllMagics(ROOK);

    // for (int i = 0; i < 64; ++i) {
    //     if (i % 8 == 0) {
    //         std::cout << std::endl;
    //     }
    //     std::cout << "0x" << std::hex << magics[i] << "," << " ";
    // }

    // return 0;

    
}