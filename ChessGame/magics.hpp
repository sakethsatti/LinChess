#pragma once
#ifndef MAGICS_HPP
#define MAGICS_HPP
#include "constants.hpp"

// Max amount of permutations for blockers is 4096
typedef array<Bitboard, 4096> BlockerTable;
typedef array<Bitboard, 4096> AttackTable;

Bitboard calcRookMask(pos square);
Bitboard calcBishopMask(pos square);

BlockerTable generateBlockerPermutations(Bitboard mask);
BlockerTable calcRookBlockers(pos square);
BlockerTable calcBishopBlockers(pos square);

Bitboard genBishopFly(const pos& square, const Bitboard& occupancy);
Bitboard genRookFly(const pos& square, const Bitboard& occupancy);

AttackTable generateAttackTable(BlockerTable blockers, Piece piece, pos square);

// This number is NOT a bitboard
U64 findMagicNumber(const BlockerTable& blockers, const int& important_bits,
                    const Piece& piece, const pos& square);
array<U64, 64> findAllMagics(Piece p);

#endif // MAGICS_HPP