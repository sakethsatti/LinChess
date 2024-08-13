#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include <chess/bitboard.hpp>
#include <chess/moves.hpp>
#include <iostream>

typedef vector<Move> LegalMoves;

class Board {
private:

  /*
        * Represents the entire position using an array of bitboards
        * 0 - pawns 
        * 1 - knights
        * 2 - bishops
        * 3 - rooks
        * 4 - queens
        * 5 - kings
        * 6 - white pieces
        * 7 - black pieces
    */
  array<Bitboard, 8> position;

  // Game info
  Color turn;
  int halfmove_clock;
  int move_number;

  // Castling rights
  bool white_king_castle;
  bool black_king_castle;
  bool white_queen_castle;
  bool black_queen_castle;
  Pos en_passant_square;

  Move orgGameState;

  Bitboard findUnsafeKingSquares(Color color);
  Bitboard attacksBySliders(Bitboard bishops, Bitboard rooks, Bitboard queens, Bitboard allPieces);
  KingAttackers findKingAttackers();
  bool in_check();
  PinnersPinned findPinnedPieces();

  const map<char, int> char_map {
    {'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'K', 5}
  };
public:  
  vector<Move> movesList;
  bool equals(const Board &b); 
  Board();
  Board(string FEN);
  void print_position();
  LegalMoves genLegalMoves();
  void moveMaker(const Move& move);
  void unmakeMove();
  void print_data();

  Color getTurn() {return turn;}

  static void print_moves(const LegalMoves& legalMove)
  {
    for (Move move : legalMove)
    {
      if (move.kc)
      {
        std::cout << "O-O" << std::endl;
      } else if (move.qc) {
        std::cout << "O-O-O" << std::endl;
      } else {
        std::cout << POS_STR[move.from] << POS_STR[move.to] << std::endl;
      }
    }
  }
};

int perft(Board &b, const int& depth, const int& oDepth);
void perftRunner(Board &b, const int& depth, const int& oDepth);
Board copier(Board b);

#endif // BOARD_HPP
