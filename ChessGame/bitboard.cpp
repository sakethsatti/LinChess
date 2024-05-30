#include "bitboard.hpp"
#include <random>

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

Bitboard pawnAttacks(pos square, Color color) {
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

Bitboard knightAttacks(pos square) {
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

Bitboard kingAttacks(pos square) {
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

Bitboard calcRookMask(pos square) {
    // XOR prevents the square from being included in the blocker possibilities
    // ~RANK_1 | ~FILE_A | ~FILE_H | ~RANK_8 are edges, which are not relevant
    
    Bitboard full_extent = (RANK_1 << (8 * (square / 8))) ^ (FILE_A << (square % 8));


    if (square/8 == 0)
    {

        if (square == 0)
            
            return full_extent & ~(RANK_8 | FILE_H);
        
        else if (square == 7)  
            
            return full_extent & ~(RANK_8 | FILE_A);

        return full_extent & ~RANK_8 & ~((FILE_A | FILE_H));
    
    } else if (square/8 == 7) {

        if (square == 56)
            
            return full_extent & ~(RANK_1 | FILE_H);
        
        else if (square == 63)  
            
            return full_extent & ~(RANK_1 | FILE_A);

        return full_extent & ~RANK_1 & ~((FILE_A | FILE_H));
    
    } else if (square%8 == 0) {

        return full_extent & ~FILE_H & ~(RANK_1 | RANK_8);

    } else if (square%8 == 7) {

        return full_extent & ~FILE_A & ~(RANK_1 | RANK_8);

    } 
    
    // If the square is not on the edge of the boardd
    return full_extent & ~(RANK_1 | FILE_A | FILE_H | RANK_8);   
    
}

Bitboard calcBishopMask(pos square) {
    int rank = square / 8;
    int file = square % 8;

    Bitboard attack = 0ULL;

    // North East
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank + i;
        int new_file = file + i;
        if (new_rank > 6 || new_file > 6) break;
        attack |= 1ULL << (new_rank * 8 + new_file);
    }

    // North West
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank + i;
        int new_file = file - i;
        if (new_rank > 6 || new_file < 1) break;
        attack |= 1ULL << (new_rank * 8 + new_file);
    }

    // South East
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank - i;
        int new_file = file + i;
        if (new_rank < 1 || new_file > 6) break;
        attack |= 1ULL << (new_rank * 8 + new_file);
    }

    // South West
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank - i;
        int new_file = file - i;
        if (new_rank < 1 || new_file < 1) break;
        attack |= 1ULL << (new_rank * 8 + new_file);
    }

    return attack;
}

BlockerTable generateBlockerPermutations(Bitboard blockers) {
    BlockerTable permutations = {0};
    Bitboard b = 0;
    int i = 0;
    do {
        permutations[i] = b;
        b = (b - blockers) & blockers;
        ++i;
    } while (b);
    return permutations;
}

BlockerTable calcRookBlockers(pos square) {
    Bitboard mask = calcRookMask(square);
    return generateBlockerPermutations(mask);
}

BlockerTable calcBishopBlockers(pos square) {
    Bitboard mask = calcBishopMask(square);
    return generateBlockerPermutations(mask);
}

Bitboard genRookFly(pos square, Bitboard occupancy)
{
    Bitboard attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    // North
    for (int i = rank + 1; i < 8; ++i) {
        int new_square = i * 8 + file;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
        
    }

    // South
    for (int i = rank - 1; i >= 0; --i) {
        int new_square = i * 8 + file;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
        
    }

    // East
    for (int i = file + 1; i < 8; ++i) {
        int new_square = rank * 8 + i;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
    }

    // West
    for (int i = file - 1; i >= 0; --i) {
        int new_square = rank * 8 + i;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
    }

    return attacks;
}

Bitboard genBishopFly(pos square, Bitboard occupancy)
{
    Bitboard attacks = 0ULL;
    int rank = square / 8;
    int file = square % 8;

    // North East
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank + i;
        int new_file = file + i;
        int new_square = new_rank * 8 + new_file;
        if (new_rank > 7 || new_file > 7) break;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
    }

    // North West
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank + i;
        int new_file = file - i;
        int new_square = new_rank * 8 + new_file;
        if (new_rank > 7 || new_file < 0) break;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
    }

    // South East
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank - i;
        int new_file = file + i;
        int new_square = new_rank * 8 + new_file;
        if (new_rank < 0 || new_file > 7) break;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
        
    }

    // South West
    for (int i = 1; i < 8; ++i) {
        int new_rank = rank - i;
        int new_file = file - i;
        int new_square = new_rank * 8 + new_file;
        if (new_rank < 0 || new_file < 0) break;
        if (occupancy & (1ULL << new_square)) break;
        attacks |= 1ULL << new_square;
    }

    return attacks;
}

AttackTable generateAttackTable(BlockerTable blockers, Piece piece, pos square)
{
    AttackTable attacks {};
    for (int i = 0; i < 4096; ++i) {
        if (!blockers[i]) continue;
        if (piece == ROOK)
            attacks[i] = genRookFly(square, blockers[i]);
        
        else 
            attacks[i] = genBishopFly(square, blockers[i]);
    }
    return attacks;
}


pair<U64, AttackTable> findMagicNumber(const BlockerTable& blockers, const AttackTable& attacks, const int& important_bits) {
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<Bitboard> distr;

    int count = 0;
    int highest = 0;
    U64 best_magic = 0;

    AttackTable test{ };

    while (true) {
        Bitboard magic = distr(eng) & distr(eng) & distr(eng);

        test = {0};
        
        bool failed = false;

        for (int i = 0; i < blockers.size(); ++i) {

            Bitboard key = (blockers[i] * magic) >> (64 - important_bits); 

            if (test[key] != 0ULL && test[key] != attacks[i]) {

                failed = true;
                break;
            }

            test[key] = attacks[i];
        }

        if (!failed) return {magic, test};

        ++count;
    }
}

