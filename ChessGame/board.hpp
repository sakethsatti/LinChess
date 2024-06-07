#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include "bitboard.hpp"

struct Move {
    
};

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


public:
    Board();
    Board(std::string FEN);
    void print_position();



};

#endif // BOARD_HPP