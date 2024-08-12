#pragma once
#ifndef MAGICS_HPP
#define MAGICS_HPP
#include "constants.hpp"

// Max amount of permutations for blockers is 4096
typedef array<Bitboard, 4096> BlockerTable;
typedef array<Bitboard, 4096> AttackTable;

Bitboard calcRookMask(const Pos& square);
Bitboard calcBishopMask(const Pos& square);
Bitboard calcRookMask(const Pos& square, const bool& edges);
Bitboard calcBishopMask(const Pos& square, const bool& edges);

BlockerTable generateBlockerPermutations(Bitboard mask);
BlockerTable calcRookBlockers(const Pos& square);
BlockerTable calcBishopBlockers(const Pos& square);

Bitboard genBishopFly(const Pos& square, const Bitboard& occupancy);
Bitboard genRookFly(const Pos& square, const Bitboard& occupancy);

// This number is NOT a bitboard
U64 findMagicNumber(const BlockerTable& blockers, const int& important_bits,
                    const Piece& piece, const Pos& square);

array<U64, 64> findAllMagics(Piece p);

#endif // MAGICS_HPP
