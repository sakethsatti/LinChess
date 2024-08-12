#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include <tuple>
#include <chess/bitboard.hpp>
#include <iostream>

using std::tuple;

typedef vector<array<Bitboard, 3>> PinnersPinned;
typedef vector<pair<Piece, Bitboard>> KingAttackers; 

struct Move {
    

    Pos from;
    Pos to;
    
    Pos en_passant_target;
    Pos en_passant_taken;  

    Color color;

    // Move was a castle
    bool kc;
    bool qc;

    // Ability to castle
    bool wkc;
    bool wqc;
    bool bkc;
    bool bqc;
  
    Piece promotion;
    Piece capture;
    bool first_move;

    Move(Pos from, Pos to, Color color, bool king_castle, bool queen_castle, Pos en_passant_target, Piece promotion,
         bool wkc = true, bool wqc = true, bool bkc = true, bool bqc = true, Piece capture = NONE, Pos en_passant_taken = SQ_NONE) :
        from(from), to(to), kc(king_castle), qc(queen_castle), color(color), en_passant_target(en_passant_target),
        promotion(promotion), wkc(wkc), wqc(wqc), bkc(bkc), bqc(bqc), capture(capture), en_passant_taken(en_passant_taken) {}

};

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
