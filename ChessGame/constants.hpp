#pragma once
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <array>
#include <map>

using std::string;
using std::vector;
using std::array;
using std::pair;
using std::map;

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
    KING,
    NONE
};

enum Pos {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    SQ_NONE
};

const array<string, 65> POS_STR = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "None"
};

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

constexpr array<int, 64> FEN_TO_POS = {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

consteval inline array<int, 64> create_index_add_table(const array<int, 64>& arr) {
    int sum = 0;
    array<int, 64> index_add_table;
    for (int i = 0; i < 64; ++i) {
        index_add_table[i] = sum;
        sum += (1 << arr[i]);
    }
    return index_add_table;
}

constexpr array<int, 64> bOffsets = create_index_add_table(bishop_relevant_bits);
constexpr array<int, 64> rOffsets = create_index_add_table(rook_relevant_bits);

// Magic numbers
constexpr U64 bishop_magics[64] = {
    0x81a0029400528200, 0x8020040102002000, 0x6016021202020000, 0x480404088644a004, 0x6204042031002b20, 0x1100844000348, 0x8009111812401008, 0xc0421001a010, 
    0x8000408808408c99, 0x4020411040500, 0x2000290801010000, 0xd80080481021400, 0x8800060210440000, 0x8000110108400108, 0x884008804108500, 0x10000c404040208, 
    0x90020802081820, 0x402001002080500, 0xa08840c001200, 0x4048050882004000, 0x803008090400010, 0x80408a00462049, 0xa800480084100840, 0x4801000084090100, 
    0x24105520200308, 0x8101182020480101, 0x1281880cd0004050, 0x4010124200880, 0x82040400401004a, 0x2808510002100200, 0x882800401314022a, 0x128c0100a1410880, 
    0x8048080808046191, 0x104020200a00401, 0x4034800490200, 0x8400a00002200, 0x4002008400420120, 0x9224010200005802, 0x8208080a40208200, 0x444008020019400, 
    0x6012020000a00, 0x1280110002481, 0x104082090120801, 0x400002011040800, 0x440f0c001200, 0x10200800400020, 0x8060a0404002114, 0x40035b0400818100, 
    0xc807821002202084, 0x10c0208410680421, 0x10000c201904600, 0x32c2428308482200, 0x140801212020081, 0x8402410901010201, 0x1040300480828806, 0x12100448808120, 
    0x200d18c90082000, 0x801a050088030808, 0x40844044400, 0x40440010020a0204, 0x4802011020a04, 0x884002882242, 0x408400504008201, 0x108010808110124
};

// Magics for bishop a1 -> h8
constexpr U64 rook_magics[64] = {
    0x18002c000846010, 0x60c004a000100140, 0x300084011002000, 0x80061000800800, 0x200200410020008, 0x200080200100481, 0x880090030802200, 0x8009000842a080, 
    0x2008800820400088, 0x8089004001042080, 0x108803004200580, 0x2800800100180, 0x101002800250030, 0xa001002000815, 0x1002000401284200, 0x2040800080006100, 
    0x480218008804000, 0x1000c2002a010280, 0x310c808010002000, 0x504808008003002, 0x808008000400, 0x2001010008122400, 0x90001400a8021001, 0x40220000408401, 
    0x80088a180004004, 0x8a04400080802000, 0x204415100200100, 0x94c284200102200, 0x207001100080014, 0x81230e0080800400, 0x22001040010080a, 0x1000011200085094, 
    0x110282400180022c, 0x2400402000c01000, 0x811202004021, 0x200890021001001, 0x4180180800400, 0x2012015812000410, 0x120a804200800100, 0x4000004082000124, 
    0x200209840008000, 0x202000d0004002, 0x8200045010050, 0x4021001003000b, 0xc285001048010014, 0x100a001008020004, 0x8080802100c0001, 0x21140250a20001, 
    0x20800040102080, 0x8010022000c00040, 0x110450020095100, 0x97300084180080, 0x800840008018080, 0x200800400060080, 0x200033208501400, 0x1000c0841108200, 
    0x4000110440208001, 0x2080110188400021, 0x100041001020024b, 0x600900009002005, 0x2e00107805200a, 0x100100040002080d, 0x4101000c12a0804, 0x840100408062
};

#endif // CONSTANTS_HPP
