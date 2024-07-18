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
int calcLSB(const Bitboard &b);
void toggleBit(Bitboard &b, Pos square);
int count_bits(Bitboard b);
U64 mask_between(int square1, int square2);

// Create look up tables
inline BishopTable createBishopTable()
{
    BishopTable table {0ULL};
    int shift = 0;
    for (int i = 0; i < 64; ++i) {
        BlockerTable blockers = calcBishopBlockers((Pos)i);
        for (int j = 0; j < (1 << bishop_relevant_bits[i]); ++j) {
            int key = (blockers[j] * bishop_magics[i]) >> (64 - bishop_relevant_bits[i]);
            key += shift;
            table[key] = genBishopFly((Pos)i, blockers[j]);
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
        BlockerTable blockers = calcRookBlockers((Pos)i);
        for (int j = 0; j < (1 << rook_relevant_bits[i]); ++j) {
            int key = (blockers[j] * rook_magics[i]) >> (64 - rook_relevant_bits[i]);
            key += shift;
            table[key] = genRookFly((Pos)i, blockers[j]);
        }
        shift += (1 << rook_relevant_bits[i]);
    }
    return table;

}

const BishopTable BISHOP_TABLE = createBishopTable();
const RookTable ROOK_TABLE = createRookTable();

// ********* Leaper pieces *********
Bitboard pawnAttacks(const Pos& square, const Color& color);
Bitboard pawnMoves(const Pos& square, const Bitboard& Blockers, const Color& color);
Bitboard knightAttacks(const Pos& square);
Bitboard kingAttacks(const Pos& square);

int find_index(const Bitboard& blockers, const Pos& sq, const Piece& piece);

// ********* Sliding pieces *********
Bitboard rookAttacks(const Pos& square, const Bitboard& board_state);
Bitboard bishopAttacks(const Pos& square, const Bitboard& board_state);
Bitboard queenAttacks(const Pos& square, const Bitboard& board_state);


#endif