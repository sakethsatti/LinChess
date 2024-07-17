#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include "bitboard.hpp"

struct Move {
    // if from or to is -1 then it is a castling move
    int from;
    int to;
    
    Color color;
    bool king_castle;
    bool queen_castle;
    // -1 if none or will be set to the square that can be captured
    int en_passant_target;

    Move(int from, int to, Color color, bool king_castle, bool queen_castle, int en_passant_target) :
        from(from), to(to), king_castle(king_castle), queen_castle(queen_castle), color(color),
        en_passant_target(en_passant_target) {}
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

    int en_passant_square;
    Bitboard findUnsafeKingSquares(Color color);
    Bitboard attacksBySliders(Bitboard bishops, Bitboard rooks, Bitboard queens, Bitboard allPieces);
    vector<pair<Piece, Bitboard>> findKingAttackers();
    bool in_check();
    

    vector<Move> move_list;
    map<char, int> char_map {
        {'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'K', 5}
    };


public:
    Board();
    Board(string FEN);
    void print_position();
    Bitboard findPinnedPieces();
    LegalMoves genLegalMoves();
    void make_move(string move);


};

#endif // BOARD_HPP