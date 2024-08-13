#include <cstdint>
#include <array>
#include <vector>

using std::array;
using std::vector;
using std::pair;

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


// Bitboard type
typedef uint64_t Bitboard;
typedef uint64_t U64;

typedef array<Bitboard, 4096> BlockerTable;
typedef array<Bitboard, 4096> AttackTable;

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

typedef vector<array<Bitboard, 3>> PinnersPinned;
typedef vector<pair<Piece, Bitboard>> KingAttackers; 

