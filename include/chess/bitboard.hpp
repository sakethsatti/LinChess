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
inline Bitboard pawnAttacks(const Pos& square, const Color& color)
{
  Bitboard attacks = 0ULL;
  if (color == WHITE) {
    if (1ULL << (square + 7) & ~FILE_H & ~RANK_1) attacks |= 1ULL << (square + 7);
    if (1ULL << (square + 9) & ~FILE_A & ~RANK_1) attacks |= 1ULL << (square + 9);
  }
  else {
    if (1ULL << (square - 9) & ~FILE_H & ~RANK_8) attacks |= 1ULL << (square - 9);
    if (1ULL << (square - 7) & ~FILE_A & ~RANK_8) attacks |= 1ULL << (square - 7);
  }
  return attacks;
}

inline Bitboard pawnMoves(const Pos& square, const Bitboard& Blockers, const Color& color)
{
  Bitboard attacks = 0ULL;

  if (color == WHITE) {
    if (1ULL << (square + 8) & ~Blockers) {
      attacks |= 1ULL << (square + 8);
      if ((1ULL << (square + 16) & ~Blockers)  && ((1ULL << square) & RANK_2)) {
        attacks |= 1ULL << (square + 16);
      }
    }
  }
  else {
    if (1ULL << (square - 8) & ~Blockers) {
      attacks |= 1ULL << (square - 8);
      if ((1ULL << (square - 16) & ~Blockers) && ((1ULL << square) & RANK_7)) {
        attacks |= 1ULL << (square - 16);
      }
    }
  }
  return attacks;
}

inline Bitboard knightAttacks(const Pos& square)
{
  Bitboard attacks = 0ULL;

  // 3 up 1 right
  if ((1ULL << (square + 17)) & ~FILE_A & ~RANK_1 & ~RANK_2) {
    attacks |= 1ULL << (square + 17);
  }

  // 3 up 1 left
  if ((1ULL << (square + 15)) & ~FILE_H & ~RANK_1 & ~RANK_2) {
    attacks |= 1ULL << (square + 15);
  }

  // 1 up 3 right
  if ((1ULL << (square + 10)) & ~(FILE_A | FILE_B) & ~RANK_1) {
    attacks |= 1ULL << (square + 10);
  }

  // 1 up 3 left
  if (1ULL << (square + 6) & ~(FILE_G | FILE_H) & ~RANK_1) {
    attacks |= 1ULL << (square + 6);
  }

  // 3 down 1 left
  if (1ULL << (square - 17) & ~FILE_H & ~RANK_7 & ~RANK_8) {
    attacks |= 1ULL << (square - 17);
  }

  // 3 down 1 right
  if (1ULL << (square - 15) & ~FILE_A & ~RANK_7 & ~RANK_8) {
    attacks |= 1ULL << (square - 15);
  }

  // 1 down 3 left
  if (1ULL << (square - 10) & ~(FILE_G | FILE_H) & ~RANK_8) {
    attacks |= 1ULL << (square - 10);
  }

  // 1 down 3 right
  if (1ULL << (square - 6) & ~(FILE_A | FILE_B) & ~RANK_8) {
    attacks |= 1ULL << (square - 6);
  }

  return attacks;

}

inline Bitboard kingAttacks(const Pos& square)
{
  Bitboard attacks = 0ULL;

  // North
  if (1ULL << (square + 8) & ~RANK_1) attacks |= 1ULL << (square + 8);

  // South
  if (1ULL << (square - 8) & ~RANK_8) attacks |= 1ULL << (square - 8);

  // East
  if (1ULL << (square + 1) & ~FILE_A) attacks |= 1ULL << (square + 1);

  // West
  if (1ULL << (square - 1) & ~FILE_H) attacks |= 1ULL << (square - 1);

  // North East
  if (1ULL << (square + 9) & ~FILE_A & ~RANK_1) attacks |= 1ULL << (square + 9);

  // Northwest
  if (1ULL << (square + 7) & ~FILE_H & ~RANK_1) attacks |= 1ULL << (square + 7);

  // Southeast
  if (1ULL << (square - 7) & ~FILE_A & ~RANK_8) attacks |= 1ULL << (square - 7);

  // Southwest
  if (1ULL << (square - 9) & ~FILE_H & ~RANK_8) attacks |= 1ULL << (square - 9);

  return attacks;

}

inline int find_index(const Bitboard& blockers, const Pos& sq, const Piece& piece)
{
  int index;
  if (piece == BISHOP) {
    index = (blockers * bishop_magics[sq]) >> (64 - bishop_relevant_bits[sq]);
    return index + bOffsets[sq];
  } else {
    index = (blockers * rook_magics[sq]) >> (64 - rook_relevant_bits[sq]);
    return index + rOffsets[sq];
  }
}

// ********* Sliding pieces *********
inline Bitboard rookAttacks(const Pos& square, const Bitboard& board_state)
{
  Bitboard attacks = 0ULL;
  Bitboard blockers = (board_state) & calcRookMask(square);
  int index = find_index(blockers, square, ROOK);
  return ROOK_TABLE[index];
}

inline Bitboard bishopAttacks(const Pos& square, const Bitboard& board_state)
{
  Bitboard attacks = 0ULL;
  Bitboard blockers = (board_state) & calcBishopMask(square);
  int index = find_index(blockers, square, BISHOP);
  return BISHOP_TABLE[index];

}

inline Bitboard queenAttacks(const Pos& square, const Bitboard& board_state)
{
  return rookAttacks(square, board_state) | bishopAttacks(square, board_state); 
}

#endif
