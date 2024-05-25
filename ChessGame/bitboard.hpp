#pragma once

#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>

using std::string;
using std::vector;
using std::array;

// Bitboard type
typedef uint64_t Bitboard;

// Max amount of permutations for blockers is 4096
typedef array<Bitboard, 4096> BlockerTable;
typedef array<Bitboard, 4096> AttackTable;

void print_bitboard(Bitboard b);

// Bitboard masks
constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_B = FILE_A << 1;
constexpr Bitboard FILE_G = FILE_A << 6;
constexpr Bitboard FILE_H = FILE_A << 7;

constexpr Bitboard RANK_1 = 0xFF;
constexpr Bitboard RANK_2 = RANK_1 << 8;
constexpr Bitboard RANK_7 = RANK_1 << 48;
constexpr Bitboard RANK_8 = RANK_1 << 56;

enum Color {
    WHITE,
    BLACK
};

enum Piece {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum pos {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

// Bitboard functions
void toggleBit(Bitboard &b, pos square);
void clear_bit(Bitboard &b, pos square);

// Normal pieces
Bitboard pawnAttacks(pos square, Color color);
Bitboard knightAttacks(pos square);
Bitboard kingAttacks(pos square);


// ********* Sliding pieces *********

// Magic number generation
Bitboard calcRookMask(pos square);
Bitboard calcBishopMask(pos square);

BlockerTable generateBlockerPermutations(Bitboard mask);
BlockerTable calcRookBlockers(pos square);
BlockerTable calcBishopBlockers(pos square);

Bitboard genRookFly(pos square, Bitboard occupancy);
Bitboard genBishopFly(pos square, Bitboard occupancy);

AttackTable generateAttackTable(BlockerTable blockers, Piece piece, pos square);

u_int64_t findMagicNumber(const BlockerTable& blockers, const AttackTable& attacks, 
                            const int important_bits, Piece piece, pos square);

// Functions to use
Bitboard rookAttacks(pos square, Bitboard occupancy);
Bitboard bishopAttacks(pos square, Bitboard occupancy);
Bitboard queenAttacks(pos square, Bitboard occupancy);

#endif