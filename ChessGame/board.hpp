#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include "bitboard.hpp"
#include <tuple>

using std::tuple;

typedef vector<array<Bitboard, 3>> PinnersPinned;
typedef vector<pair<Piece, Bitboard>> KingAttackers; 

struct Move {
    Pos from;
    Pos to;
    
    Pos en_passant_target;

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
    bool first_move;

    Move(Pos from, Pos to, Color color, bool king_castle, bool queen_castle, Pos en_passant_target, Piece promotion,
         bool wkc = true, bool wqc = true, bool bkc = true, bool bqc = true, bool first_move = false) :
        from(from), to(to), kc(king_castle), qc(queen_castle), color(color), en_passant_target(en_passant_target),
        promotion(promotion), wkc(wkc), wqc(wqc), bkc(bkc), bqc(bqc), first_move(first_move) {}
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

    vector<Move> movesList;

    Bitboard findUnsafeKingSquares(Color color);
    Bitboard attacksBySliders(Bitboard bishops, Bitboard rooks, Bitboard queens, Bitboard allPieces);
    KingAttackers findKingAttackers();
    bool in_check();
    PinnersPinned findPinnedPieces();

    map<char, int> char_map {
        {'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'K', 5}
    };
public:
    Board();
    Board(string FEN);
    void print_position();
    LegalMoves genLegalMoves();
    void moveMaker(const Move& move);
    void unmakeMove();
};

#endif // BOARD_HPP
