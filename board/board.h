#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

#define RANKS 8

using std::array;

typedef array<int, 8> files;
typedef array<files, RANKS> EncodingBoard;

class Board {
    // Normal chess board with chars
    array<array<char, 8>, 8> board_array;
    
    // King info
    bool player_king_moved;
    bool opp_king_moved;
    bool player_rook_kingside_moved;
    bool player_rook_queenside_moved;
    bool opp_rook_kingside_moved;
    bool opp_rook_queenside_moved;
    bool in_check;

    int moves_since_last_capture;
    int moves_since_last_pawn_move;

    // Important info
    char turn;

    // Individual piece move handlers
    void move_pawn(int rank, int file, int new_rank, int new_file, char player);
    void move_knight(int rank, int file, int new_rank, int new_file, char player);
    void move_bishop(int rank, int file, int new_rank, int new_file, char player);
    void move_rook(int rank, int file, int new_rank, int new_file, char player);
    void move_queen(int rank, int file, int new_rank, int new_file, char player);
    void move_king(int rank, int file, int new_rank, int new_file, char player);
    

public:
    Board();
    void fill_board();
    void print_board();
    
    void play_move(int rank, int file, int new_rank, int new_file);
};

#endif // BOARD_H