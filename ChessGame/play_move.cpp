#include "board.hpp"
#include <stdexcept>

// Function to play a move
void Board::move_piece(int old_file, int old_rank, int new_file, int new_rank) {
    
    // Check if move is outside of the allowed squares
    if (old_file < 0 || old_file >= FILES ||
        old_rank < 0 || old_rank >= RANKS ||
        new_file < 0 || new_file >= FILES ||
        new_rank < 0 || new_rank >= RANKS) {
        throw std::invalid_argument("Invalid move: out of bounds");
    }

    // Check if source square is empty
    if (board_repr[old_file][old_rank].empty) {
        throw std::invalid_argument("Invalid move: no piece in source square");
    }

    // Check if player is attempting to move an opposing piece
    if (board_repr[old_file][old_rank].piece.color != turn) {
        throw std::invalid_argument("Invalid move: not your turn");
    }

    // Check if a player is trying to take their own piece
    if (!board_repr[new_file][new_rank].empty && board_repr[new_file][new_rank].piece.color == turn) {
        throw std::invalid_argument("Invalid move: cannot take your own piece");
    }


    
    // Move piece
    board_repr[new_file][new_rank] = board_repr[old_file][old_rank];
    board_repr[old_file][old_rank] = Square(EMPTY, NONE);
    
    // Change turn
    turn = (turn == WHITE) ? BLACK : WHITE;
}

/*
    * Function to check if the player is in check
    * Notice the use of board param rather than board_repr
    * This will be useful when checking for a pin.
*/
bool Board::is_in_check(const SquareArray& board) {
    // Find the king
    int king_file = -1;
    int king_rank = -1;
    for (int file = 0; file < FILES; file++) {
        for (int rank = 0; rank < RANKS; rank++) {
            if (board[file][rank].piece.type == KING && board[file][rank].piece.color == turn) {
                king_file = file;
                king_rank = rank;
            }
        }
    }

    // This will prevent going out of bounds on the diagonals
    int& smaller = (king_file < king_rank) ? king_file : king_rank;
    array<int, 2> start_left = {king_file - smaller, king_rank - smaller};
    array<int, 2> start_right = {king_file - smaller, king_rank - smaller};

    // Check if the king is in check diagonally
    for (int i = 0; i < 8; ++i)
    {
        
    }



    return false;
}