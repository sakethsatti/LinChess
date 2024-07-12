#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include "bitboard.hpp"

struct Move {
    pos from;
    pos to;
    Color color;
    bool king_moved;
    bool kingside_rook_moved;
    bool queenside_rook_moved;
    bool en_passant_possible;
    pos en_passant_target;

    Move(pos from, pos to, Color color, bool king_moved, bool kingside_rook_moved, bool queenside_rook_moved) : 
        from(from), to(to), color(color), king_moved(king_moved), kingside_rook_moved(kingside_rook_moved),
        queenside_rook_moved(queenside_rook_moved) {}

    Move(pos from, pos to, Color color, bool king_moved, bool kingside_rook_moved, bool queenside_rook_moved, pos en_passant_target) : 
        from(from), to(to), color(color), king_moved(king_moved), kingside_rook_moved(kingside_rook_moved),
        queenside_rook_moved(queenside_rook_moved), en_passant_possible(true), en_passant_target(en_passant_target) {}
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
    bool in_check(Color color);

    vector<Move> move_list;
    map<char, int> char_map {
        {'P', 0}, {'N', 1}, {'B', 2}, {'R', 3}, {'Q', 4}, {'K', 5}
    };

    LegalMoves genLegalMoves();


public:
    Board();
    Board(string FEN);
    void print_position();
    void make_move(string move);


};

#endif // BOARD_HPP