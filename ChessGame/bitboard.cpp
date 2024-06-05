#include "bitboard.hpp"

void print_bitboard(Bitboard b)
{
    for (int rank = 7; rank >= 0; --rank)
    {
        std::cout <<  rank + 1 << "   ";
        for (int file = 0; file < 8; ++file)
        {
            int square = rank * 8 + file;
            
            // Bitshift 1 left in order to check if there is a 1 in the square 
            std::cout << (b & (1ULL << square) ? 1 : 0) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "    A B C D E F G H" << std::endl;
    std::cout << std::endl;
}

void toggleBit(Bitboard &b, pos square) {
    b ^= 1ULL << square;
}

void clearBit(Bitboard &b, pos square) {
    b &= ~(1ULL << square);
}

int count_bits(Bitboard number) {
    int count = 0;
    while (number) {
        number &= (number - 1);
        count++;
    }
    return count;
}

Bitboard pawnAttacks(const pos& square, const Color& color) {
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

Bitboard knightAttacks(const pos& square) {
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

Bitboard kingAttacks(const pos& square) {
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


// This will help find the index in the massive lookup tables
int find_index(const Bitboard& blockers, const pos& sq, const Piece& piece) {
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
Bitboard rookAttacks(const pos& square, const Bitboard& allies, const Bitboard& opps)
{
    Bitboard attacks = 0ULL;
    Bitboard blockers = (allies | opps) & calcRookMask(square);
    int index = find_index(blockers, square, ROOK);
    return ROOK_TABLE[index] & ~allies;
}

Bitboard bishopAttacks(const pos& square, const Bitboard& allies, const Bitboard& opps)
{
    Bitboard attacks = 0ULL;
    Bitboard blockers = (allies | opps) & calcBishopMask(square);
    int index = find_index(blockers, square, BISHOP);
    return BISHOP_TABLE[index] & ~allies;
}

Bitboard queenAttacks(const pos& square, const Bitboard& allies, const Bitboard& opps)
{
    return rookAttacks(square, allies, opps) | bishopAttacks(square, allies, opps);
}