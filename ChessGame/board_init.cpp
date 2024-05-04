#include "board.hpp"
#include <iostream>

Board::Board()
{
    // Initalize starting position
    turn = WHITE;

    // Generate Pawns
    for ( int i = 0; i < FILES; i++ )
    {
        board_repr[i][1] = Square(PAWN, WHITE);
        board_repr[i][6] = Square(PAWN, BLACK);
    }

    // Generate Knights
    board_repr[1][0] = Square(KNIGHT, WHITE);
    board_repr[6][0] = Square(KNIGHT, WHITE);
    board_repr[1][7] = Square(KNIGHT, BLACK);
    board_repr[6][7] = Square(KNIGHT, BLACK);

    // Generate Bishops
    board_repr[2][0] = Square(BISHOP, WHITE);
    board_repr[5][0] = Square(BISHOP, WHITE);
    board_repr[2][7] = Square(BISHOP, BLACK);
    board_repr[5][7] = Square(BISHOP, BLACK);

    // Generate Rooks
    board_repr[0][0] = Square(ROOK, WHITE);
    board_repr[7][0] = Square(ROOK, WHITE);
    board_repr[0][7] = Square(ROOK, BLACK);
    board_repr[7][7] = Square(ROOK, BLACK);

    // Generate Queens
    board_repr[3][0] = Square(QUEEN, WHITE);
    board_repr[3][7] = Square(QUEEN, BLACK);

    // Generate Kings
    board_repr[4][0] = Square(KING, WHITE);
    board_repr[4][7] = Square(KING, BLACK);
}

void Board::print_board()
{
    for ( int i = RANKS - 1; i >= 0; i-- )
    {
        for ( int j = 0; j < FILES; j++ )
        {
            char toPrint = PIECE_TO_CHAR.at(board_repr[j][i].piece.type);
            if (board_repr[j][i].piece.color == WHITE)
            {
                std::cout << toPrint << " ";
            }
            else if (board_repr[j][i].piece.color == BLACK)
            {
                std::cout << (char)(toPrint - 'A' + 'a') << " ";
            }
        }
        std::cout << std::endl;
    }
}