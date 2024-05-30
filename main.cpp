#include <iostream>
#include <bitset>
#include "ChessGame/bitboard.hpp"

int main() {    
    pos square = e4;
    BlockerTable blockers = calcRookBlockers(square);
    AttackTable attacks = generateAttackTable(blockers, ROOK, square);
    auto [magic, table] = findMagicNumber(blockers, attacks, rook_relevant_bits[square]);
    // std::bitset<64> y(magic);
    // std::cout << y << std::endl;

    Bitboard b = blockers[233];

    print_bitboard(b);
    int index = (b * magic) >> (64 - rook_relevant_bits[square]);
    std::cout << index << std::endl;
    print_bitboard(table[index]);

    return 0;
}