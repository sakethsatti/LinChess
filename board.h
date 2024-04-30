#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

#define RANKS 8

using std::array;

typedef array<int, 8> files;
typedef array<files, RANKS> board;

class Board{
    
    /*
    * Define 18 x 8 x 8 datastructure
    * Each 8 x 8 array represents a plane of the board
    * The 18 planes represent different features of the board
    * 0 & 10: position of the king on the board (0: player, 10: opponent)
    * 1 & 11: position of the pawns (1: player, 11: opponent)
    * 2 & 12: position of the knights (2: player, 12: opponent)
    * 3 & 13: position of the bishops (3: player, 13: opponent)
    * 4 & 14: position of the rooks (4: player, 14: opponent)
    * 5 & 15: position of the queens (5: player, 15: opponent)
    * 6 & 16: kingside castling rights (6: player, 16: opponent)
    * 7 & 17: queenside castling rights (7: player, 17: opponent) 
    */ 

    array<board, 18> board_planes;
    array<array<char, 8>, 8> board_array;
    

public:
    Board();
    void fillBoard();
    void printBoard();
    
};

#endif // BOARD_H