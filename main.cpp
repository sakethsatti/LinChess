#include <iostream>
#include <array>
#include <string>
#include "ChessGame/bitboard.hpp"

int main() {    
    BlockerTable blockers = calcBishopBlockers(e4);
    AttackTable bishAttacks = generateAttackTable(blockers, BISHOP, e4);

    print_bitboard(blockers[233]);
    print_bitboard(bishAttacks[233]);

    // Bitboard stuff = genBishopFly(e4, blockers[233]);
    // print_bitboard(blockers[233]);
    // print_bitboard(stuff);    

    return 0;
}