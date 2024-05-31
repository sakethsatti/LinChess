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
using std::pair;

// Bitboard type
typedef uint64_t Bitboard;
typedef uint64_t U64;

// Bitboard masks
constexpr Bitboard FILE_A = 0x0101010101010101ULL;
constexpr Bitboard FILE_B = FILE_A << 1;
constexpr Bitboard FILE_G = FILE_A << 6;
constexpr Bitboard FILE_H = FILE_A << 7;

constexpr Bitboard RANK_1 = 0xFF;
constexpr Bitboard RANK_2 = RANK_1 << 8;
constexpr Bitboard RANK_7 = RANK_1 << 48;
constexpr Bitboard RANK_8 = RANK_1 << 56;

constexpr array<int, 64> bishop_relevant_bits = {
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
constexpr array<int, 64> rook_relevant_bits = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

inline consteval int sum_relevant_bits(const array<int, 64>& relevant_bits) {
    int result = 0;
    for (int i = 1; i < 64; ++i) {
        result += 1 << relevant_bits[i];
    }
    return result;
}

// Bishop look up size and Rook look up size
constexpr int BLS = sum_relevant_bits(bishop_relevant_bits);
constexpr int RLS = sum_relevant_bits(rook_relevant_bits);

// Create look up tables
inline consteval array<Bitboard, BLS> createBishopTable()
{
    array<Bitboard, BLS> table;
    for (int square = 0; square < 64; ++square)
    {
        int key = ()
        table[]
    }
}

consteval array<U64, RLS> createRookTable();

// Magic numbers
constexpr U64 rook_magics[64] = {
    0x480014001102c80, 0x40100040002000, 0x8020002a100081, 0x83000810000d00a0, 0x42001058a0020004, 0x8002002d804400, 0x6400040802089001, 0x500004100002182,
    0x210802040008008, 0x4000400040601000, 0x601002001930040, 0x1002801002080084, 0x3800401800800, 0xc000800400a20080, 0x1002000200042188, 0x9163000180e20100,
    0x808888008204000, 0x800c808020004010, 0x4002110020010240, 0x200a0200208810c0, 0x80080800c000802, 0x2000808002000400, 0x40004001a081001, 0x1800220002529104,
    0x10800080214002, 0x9210200c4001d000, 0x1020008380500020, 0x400080280100180, 0x10080080040080, 0x120440801201040, 0x80400300225, 0x8008800ac100,
    0x1000400080800021, 0x4030002001c000c0, 0x810022400a00800, 0x400800800801002, 0x100040080800801, 0x4e1802400804200, 0x4040100804000102, 0x4820801040800500,
    0x806040008000, 0x890114020024000, 0x10002000838050, 0x800300a010010008, 0x214080100250010, 0x5802000850020004, 0x6040700108c40002, 0x20801000401020,
    0xc00218000400480, 0x3004c00031068100, 0x6480100a200080, 0x8000409160ba0200, 0x8028020c0080, 0x8010002a4000900, 0x5002200840100, 0x208000e5000480,
    0x80002031008841, 0x408610080400875, 0x6022040810832, 0xc41200106040444a, 0x2002488102046, 0x2001081080402, 0x6802002400b8090a, 0x20211410020840a
};

// Magics for bishop a1 -> h8
constexpr U64 bishop_magics[64] = {
    0x80008060400011, 0x4000a0005001c0, 0x2500200100104108, 0x2180100006801800, 0x80120400802800, 0x100040002281900, 0x600009304020008, 0x4980042043000480,
    0x8000a4804000, 0x1004000892500, 0x2004062008034, 0x2c0100081000a100, 0x41000802300500, 0x800c00020080, 0x8000808015000200, 0x49001220408100, 
    0x2000808000604008, 0xe830044011a00041, 0x1120808020001001, 0x18008010008408, 0x110015000800, 0x70100222c0008, 0x10484000108700a, 0xa0000810844, 
    0x2080004040002000, 0x10900340c0002000, 0x4010a200408200, 0x3030015080080181, 0x8802800c0080, 0x86400c0080220080, 0x200301400081a81, 0x20800880025100, 
    0x808020400080088a, 0x890201000400140, 0xb420008d801000, 0x100c182501001000, 0x81001801000470, 0x4002002004040010, 0x1000280104000610, 0x200001a412000041, 
    0x102080a4c0008000, 0x8000200050004000, 0x10008120048011, 0x22a0500100090020, 0x202002460120018, 0x1000204010008, 0x180012248040030, 0x201004100b20004, 
    0x8204288208410200, 0x10124007200040, 0x400101200218c300, 0x800800300080, 0x1800403080080, 0x2822000990040200, 0x900080281100c00, 0x1580004304008200, 
    0x8000604c10800101, 0x20201280400505, 0x430200804010204a, 0x288500020093d01, 0x4200102800050105, 0x81000804008201, 0x81002082002401, 0x600022404410982
};



void print_bitboard(Bitboard b);

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

// Bitboard functions
void toggleBit(Bitboard &b, pos square);
int count_bits(Bitboard b);

// ********* Leaper pieces *********
Bitboard pawnAttacks(pos square, Color color);
Bitboard knightAttacks(pos square);
Bitboard kingAttacks(pos square);

// ********* Sliding pieces *********
Bitboard rookAttacks(pos square, Bitboard occupancy);
Bitboard bishopAttacks(pos square, Bitboard occupancy);
Bitboard queenAttacks(pos square, Bitboard occupancy);


#endif