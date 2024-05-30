#pragma once
#ifndef MAGICS_HPP
#define MAGICS_HPP
#include "bitboard.hpp"

inline Bitboard calcRookMask(pos square);
inline Bitboard calcBishopMask(pos square);

inline BlockerTable generateBlockerPermutations(Bitboard mask);
inline BlockerTable calcRookBlockers(pos square);
inline BlockerTable calcBishopBlockers(pos square);

inline Bitboard genRookFly(pos square, Bitboard occupancy);
inline Bitboard genBishopFly(pos square, Bitboard occupancy);

inline AttackTable generateAttackTable(BlockerTable blockers, Piece piece, pos square);

// This number is NOT a bitboard
inline U64 findMagicNumber(const BlockerTable& blockers, const AttackTable& attacks, const int& important_bits);
array<U64, 64> findAllMagics(Piece p);

#endif // MAGICS_HPP