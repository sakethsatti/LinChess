#pragma once

#ifndef BITBOARD_HPP
#define BITBOARD_HPP
#include <cstdint>
#include <string>
#include <iostream>

using std::string;

// Bitboard type
typedef uint64_t Bitboard;

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

// Bitboard functions
void toggle_bit(Bitboard &b, pos square);
void clear_bit(Bitboard &b, pos square);

// Normal pieces
Bitboard pawn_attacks(pos square, Color color);
Bitboard knight_attacks(pos square);
Bitboard king_attacks(pos square);

//Sliding pieces
Bitboard rook_attacks(pos square);
Bitboard bishop_attacks(pos square);
Bitboard queen_attacks(pos square);

#endif