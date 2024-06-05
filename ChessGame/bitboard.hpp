#pragma once

#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include "constants.hpp"
#include "magics.hpp"

inline consteval int sum_relevant_bits(const array<int, 64>& relevant_bits) {
    int result = 0;
    for (int i = 0; i < 64; ++i) {
        result += 1 << relevant_bits[i];
    }
    return result;
}

// Bishop look up size and Rook look up size
constexpr int BLS = sum_relevant_bits(bishop_relevant_bits);
constexpr int RLS = sum_relevant_bits(rook_relevant_bits);

typedef array<Bitboard, BLS> BishopTable;
typedef array<Bitboard, RLS> RookTable;

void print_bitboard(Bitboard b);

// Bitboard functions
void toggleBit(Bitboard &b, pos square);
int count_bits(Bitboard b);

// Create look up tables
inline BishopTable createBishopTable()
{
    BishopTable table {0ULL};
    int shift = 0;
    for (int i = 0; i < 64; ++i) {
        BlockerTable blockers = calcBishopBlockers((pos)i);
        for (int j = 0; j < (1 << bishop_relevant_bits[i]); ++j) {
            int key = (blockers[j] * bishop_magics[i]) >> (64 - bishop_relevant_bits[i]);
            key += shift;
            table[key] = genBishopFly((pos)i, blockers[j]);
        }
        shift += (1 << bishop_relevant_bits[i]);
    }
    return table;
}


inline RookTable createRookTable()
{
    RookTable table {0ULL};
    int shift = 0;
    for (int i = 0; i < 64; ++i) {
        BlockerTable blockers = calcRookBlockers((pos)i);
        for (int j = 0; j < (1 << rook_relevant_bits[i]); ++j) {
            int key = (blockers[j] * rook_magics[i]) >> (64 - rook_relevant_bits[i]);
            key += shift;
            table[key] = genRookFly((pos)i, blockers[j]);
        }
        shift += (1 << rook_relevant_bits[i]);
    }
    return table;

}

const BishopTable BISHOP_TABLE = createBishopTable();
const RookTable ROOK_TABLE = createRookTable();

// ********* Leaper pieces *********
Bitboard pawnAttacks(const pos& square, const Color& color);
Bitboard knightAttacks(const pos& square);
Bitboard kingAttacks(const pos& square);

int find_index(const Bitboard& blockers, const pos& sq, const Piece& piece);

// ********* Sliding pieces *********
Bitboard rookAttacks(const pos& square, const Bitboard& allies, const Bitboard& opps);
Bitboard bishopAttacks(const pos& square, const Bitboard& allies, const Bitboard& opps);
Bitboard queenAttacks(const pos& square, const Bitboard& allies, const Bitboard& opps);


#endif