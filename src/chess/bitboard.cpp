#include <chess/bitboard.hpp>
#include <stdexcept>
#include <random>
#include <iostream>

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

// Least significant bit
int calcLSB(const Bitboard &b)
{
    return __builtin_ctzll(b);
}

void toggleBit(Bitboard &b, Pos square) {
    b ^= 1ULL << square;
}

void clearBit(Bitboard &b, Pos square) {
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

U64 mask_between(int square1, int square2) {
    
    U64 mask = 0ULL;
    int direction = 0;

    const int& lower = std::min(square1, square2);
    const int& upper = std::max(square1, square2);

    // Determine direction
    if (square1 / 8 == square2 / 8)      direction = 1;  // Same rank
    else if (square1 % 8 == square2 % 8) direction = 8;  // Same file
    else if (abs(square1 - square2) % 9 == 0) direction = 9;  // Diagonal
    else if (abs(square1 - square2) % 7 == 0) direction = 7;  // Diagonal
    else throw std::invalid_argument("Invalid squares");
    
    for (int square = lower + direction; square != upper; square += direction) {
        mask |= 1ULL << square;
    }

    return mask;

}

Bitboard calcRookMask(const Pos& square, const bool& edge) {
  // XOR prevents the square from being included in the blocker possibilities

  return (RANK_1 << (8 * (square / 8))) ^ (FILE_A << (square % 8));
}

Bitboard calcRookMask(const Pos& square) {
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

Bitboard calcBishopMask(const Pos& square, const bool& edges) {
  int rank = square / 8;
  int file = square % 8;

  Bitboard attack = 0ULL;

  // North East
  if (rank != 7 && file != 7) {
    for (int i = 1; i < 8; ++i) {
      int new_rank = rank + i;
      int new_file = file + i;
      attack |= 1ULL << (new_rank * 8 + new_file);
      if (new_rank > 6 || new_file > 6) break;
    }
  }

  // North West 
  if (rank != 7 && file != 0) {
    for (int i = 1; i < 8; ++i) {
      int new_rank = rank + i;
      int new_file = file - i;
      attack |= 1ULL << (new_rank * 8 + new_file);
      if (new_rank > 6 || new_file < 1) break;
    }
  }

  // South East
  if (rank != 0 && file != 7){
    for (int i = 1; i < 8; ++i) {
      int new_rank = rank - i;
      int new_file = file + i;
      attack |= 1ULL << (new_rank * 8 + new_file);
      if (new_rank < 1 || new_file > 6) break;
    }
  }

  // South West
  if (rank != 0 && file != 0){
    for (int i = 1; i < 8; ++i) {
      int new_rank = rank - i;
      int new_file = file - i;
      attack |= 1ULL << (new_rank * 8 + new_file);
      if (new_rank < 1 || new_file < 1) break;
    }
  }

  return attack;
}

Bitboard calcBishopMask(const Pos& square) {
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

BlockerTable calcRookBlockers(const Pos& square) {
  Bitboard mask = calcRookMask(square);
  return generateBlockerPermutations(mask);
}

BlockerTable calcBishopBlockers(const Pos& square) {
  Bitboard mask = calcBishopMask(square);
  return generateBlockerPermutations(mask);
}

Bitboard genRookFly(const Pos& square, const Bitboard& occupancy)
{
  Bitboard attacks = 0ULL;
  int rank = square / 8;
  int file = square % 8;

  // North
  for (int i = rank + 1; i < 8; ++i) {
    int new_square = i * 8 + file;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;
  }

  // South
  for (int i = rank - 1; i >= 0; --i) {
    int new_square = i * 8 + file;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;

  }

  // East
  for (int i = file + 1; i < 8; ++i) {
    int new_square = rank * 8 + i;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;
  }

  // West
  for (int i = file - 1; i >= 0; --i) {
    int new_square = rank * 8 + i;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;
  }

  return attacks;
}

Bitboard genBishopFly(const Pos& square, const Bitboard& occupancy)
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
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;
  }

  // North West
  for (int i = 1; i < 8; ++i) {
    int new_rank = rank + i;
    int new_file = file - i;
    int new_square = new_rank * 8 + new_file;
    if (new_rank > 7 || new_file < 0) break;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;
  }

  // South East
  for (int i = 1; i < 8; ++i) {
    int new_rank = rank - i;
    int new_file = file + i;
    int new_square = new_rank * 8 + new_file;
    if (new_rank < 0 || new_file > 7) break;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;

  }

  // South West
  for (int i = 1; i < 8; ++i) {
    int new_rank = rank - i;
    int new_file = file - i;
    int new_square = new_rank * 8 + new_file;
    if (new_rank < 0 || new_file < 0) break;
    attacks |= 1ULL << new_square;
    if (occupancy & (1ULL << new_square)) break;
  }

  return attacks;
}

U64 findMagicNumber(const BlockerTable& blockers, const int& important_bits, const Piece& piece, const Pos& square) {
  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<Bitboard> distr;

  AttackTable test{ };

  while (true) {
    Bitboard magic = distr(eng) & distr(eng) & distr(eng);

    test = {0};

    bool failed = false;

    for (int i = 0; i < blockers.size(); ++i) {

      Bitboard key = (blockers[i] * magic) >> (64 - important_bits);
      Bitboard attacks = (piece == ROOK) ? genRookFly(square, blockers[i]) : genBishopFly(square, blockers[i]);

      if (test[key] && test[key] != attacks) {
        failed = true;
        break;
      }

      test[key] = attacks;
    }

    if (!failed) return magic;
  }
}

array<U64, 64> findAllMagics(Piece p)
{
  array<U64, 64> magics {0};
  for (int i = 0; i < 64; ++i) {

    if (i % 10 == 0) std::cout << i << std::endl;

    BlockerTable blockers = (p == BISHOP) ? calcBishopBlockers((Pos)i) : calcRookBlockers((Pos)i);

    if (p == BISHOP)
    {
      U64 magic = findMagicNumber(blockers, bishop_relevant_bits[i], BISHOP, (Pos)i);
      magics[i] = magic;
    } 

    else {
      U64 magic = findMagicNumber(blockers, rook_relevant_bits[i], ROOK, (Pos)i);
      magics[i] = magic;
    }
  }
  return magics;
}
