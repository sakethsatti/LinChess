#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <ostream>

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

constexpr Color operator~(Color c) { return Color(c ^ BLACK);}

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

enum MoveType {
  NORMAL,
  PROMOTION,
  EN_PASSANT,
  CASTLING
};

struct CastlingInfo {
  uint8_t castleRepr; 
  CastlingInfo(const bool& wkc, const bool& wqc, const bool& bkc, const bool& bqc) {
    castleRepr = 0;
    castleRepr |= wkc;
    castleRepr |= wqc << 1;
    castleRepr |= bkc << 2;
    castleRepr |= bqc << 3;
  }

  inline bool wkc() const {
    return 1 & castleRepr;
  }

  inline bool wqc() const {
    return 2 & castleRepr;
  }

  inline bool bkc() const {
    return 4 & castleRepr;
  }

  inline bool bqc() const {
    return 8 & castleRepr;
  }
  
  inline void setWKC(const bool& b) {
    if (b) {
      castleRepr |= 1;
      return;
    }
    
    castleRepr &= ~1;
  }

  inline void setWQC(const bool& b) {
    if (b) {
      castleRepr |= 2;
      return;
    }
    
    castleRepr &= ~2;
  } 
  
  inline void setWC(const bool& togWKC, const bool& togWQC) {
    setWKC(togWKC);
    setWQC(togWQC);
  }

  inline void setBKC(const bool& b) {
    if (b) {
      castleRepr |= 4;
      return;
    }
    
    castleRepr &= ~4;
  }

  inline void setBQC(const bool& b) {
    if (b) {
      castleRepr |= 8;
      return;
    }
    
    castleRepr &= ~8;
  } 
  
  inline void setBC(const bool& togWKC, const bool& togWQC) {
    setBKC(togWKC);
    setBQC(togWQC);
  }


  friend std::ostream& operator<<(std::ostream& os, const CastlingInfo& ci);
  bool operator==(const CastlingInfo& ci) const; 
};

inline bool CastlingInfo::operator==(const CastlingInfo& ci) const {
  return castleRepr == ci.castleRepr;
}

inline std::ostream& operator<<(std::ostream& os, const CastlingInfo& ci){
  os << ((ci.wkc()) ? "K" : "")
     << ((ci.wqc()) ? "Q" : "")
     << ((ci.bkc()) ? "k" : "")
     << ((ci.bqc()) ? "q" : "");

  return os;
}

// Bitboard type
using Bitboard = uint64_t;
using U64 = uint64_t;

using BlockerTable = array<Bitboard, 4096>;
using AttackTable = array<Bitboard, 4096>;

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

using BishopTable = array<Bitboard, BLS>;
using RookTable = array<Bitboard, RLS>;

using PinnersPinned = vector<array<Bitboard, 3>>;
using KingAttackers = vector<pair<Piece, Bitboard>>;

/*
  * Bits represent different things
  * 0-5: from
  * 6-11: to
    * Also stores castle (two square move by king)
  * 12-13: promotion type
  * 14-15: 01 promotion, 10 en passant, 11 castle
*/
struct Move { 
  uint16_t moveInfo; 

  constexpr inline Pos from () const {
    return (Pos)(moveInfo & 0x3F);
  }

  constexpr inline Pos to () const {
    return (Pos)((moveInfo >> 6) & 0x3F);
  }

  constexpr inline Piece promotion_type() const {
    return (moveType() == PROMOTION) ? Piece(((moveInfo >> 12) & 3) + 1) : Piece::NONE;
  } 

  constexpr inline MoveType moveType() const {
    return (MoveType)(moveInfo >> 14);
  }

  template<MoveType MT>
  static constexpr Move create(const Pos& from, const Pos& to, const Piece& promo = KNIGHT) {
    return Move((MT << 14) + ((promo - KNIGHT) << 12) + (to << 6) + from);
  } 

  constexpr explicit Move(const uint16_t& info): moveInfo(info) {};
};

using MovesList = vector<Move>;

