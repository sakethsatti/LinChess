#include "board.hpp"
#include <iostream>

// TODO: Split this function into smaller functions for better readability

// Generating the attack bitboards is useful for determing if a move is legal
BitBoardAttack Board::generate_attack_bitboards(SquareArray& input_board, Color c)
{

    // fdv = four directions validity
    // Bishop and Rook
    array<bool, 4> fdv;
    // edv = eight direction validity
    // Queen
    array<bool, 8> edv;
    // How far out the attack has extended
    int extent;

    BitBoardAttack attack_bitboard;

    
    for ( int i = 0; i < FILES; i++ )
    {
        for ( int j = 0; j < RANKS; j++ )
        {
            if (input_board[i][j].piece.color == c)
            {
                switch (input_board[i][j].piece.type)
                {
                    case PAWN:
                        if (c == WHITE)
                        {
                            if (valid_index(i-1, j+1))
                            {
                                attack_bitboard[i-1][j+1] = 1;
                            }
                            if (valid_index(i+1, j+1))
                            {
                                attack_bitboard[i+1][j+1] = 1;
                            }
                        } else { // If it is a black piece, the pawns should take it the opposite direction
                            if (valid_index(i-1, j-1))
                            {
                                attack_bitboard[i-1][j-1] = 1;
                            }
                            if (valid_index(i+1, j-1))
                            {
                                attack_bitboard[i+1][j-1] = 1;
                            }
                        }
                        break;
                    case ROOK:
                        // fdv = four directions validity
                        fdv = {true, true, true, true};
                        extent = 1;
                        while (fdv[0] || fdv[1] || fdv[2] || fdv[3])
                        {
                            if (fdv[0] && valid_index(i, j+extent))
                            { 
                                attack_bitboard[i][j+extent] = 1;
                                if (!(input_board[i][j+extent].empty))
                                {
                                    fdv[0] = false;
                                }
                            }
                            else
                            {
                                fdv[0] = false;
                            }

                            if (fdv[1] && valid_index(i, j-extent))
                            {
                                attack_bitboard[i][j-extent] = 1;
                                if (!input_board[i][j-extent].empty)
                                {
                                    fdv[1] = false;
                                }
                            }
                            else
                            {
                                fdv[1] = false;
                            }

                            if (fdv[2] && valid_index(i+extent, j))
                            {
                                attack_bitboard[i+extent][j] = 1;
                                if (!input_board[i+extent][j].empty)
                                {
                                    fdv[2] = false;
                                }
                            }
                            else
                            {
                                fdv[2] = false;
                            }

                            if (fdv[3] && valid_index(i-extent, j))
                            {
                                attack_bitboard[i-extent][j] = 1;
                                if (!input_board[i-extent][j].empty)
                                {
                                    fdv[3] = false;
                                }
                            }
                            else
                            {
                                fdv[3] = false;
                            }

                            extent++;
                        }
                        break;
                    case KNIGHT:
                        if (valid_index(i+3, j+1))
                        {
                            attack_bitboard[i+2][j+1] = 1;
                        }
                        if (valid_index(i+3, j-1))
                        {
                            attack_bitboard[i+2][j-1] = 1;
                        }
                        if (valid_index(i-3, j+1))
                        {
                            attack_bitboard[i-2][j+1] = 1;
                        }
                        if (valid_index(i-3, j-1))
                        {
                            attack_bitboard[i-2][j-1] = 1;
                        }
                        if (valid_index(i+1, j+3))
                        {
                            attack_bitboard[i+1][j+2] = 1;
                        }
                        if (valid_index(i+1, j-3))
                        {
                            attack_bitboard[i+1][j-2] = 1;
                        }
                        if (valid_index(i-1, j+3))
                        {
                            attack_bitboard[i-1][j+2] = 1;
                        }
                        if (valid_index(i-1, j-3))
                        {
                            attack_bitboard[i-1][j-2] = 1;
                        }
                        break;
                    case BISHOP:
                        // fdv = four directions validity
                        fdv = {true, true, true, true};
                        extent = 1;
                        while (fdv[0] || fdv[1] || fdv[2] || fdv[3])
                        {
                            if (fdv[0] && valid_index(i+extent, j+extent))
                            { 
                                attack_bitboard[i+extent][j+extent] = 1;
                                if (!(input_board[i+extent][j+extent].empty))
                                {
                                    fdv[0] = false;
                                }
                            }
                            else
                            {
                                fdv[0] = false;
                            }

                            if (fdv[1] && valid_index(i-extent, j-extent))
                            {
                                attack_bitboard[i-extent][j-extent] = 1;
                                if (!input_board[i-extent][j-extent].empty)
                                {
                                    fdv[1] = false;
                                }
                            }
                            else
                            {
                                fdv[1] = false;
                            }

                            if (fdv[2] && valid_index(i+extent, j-extent))
                            {
                                attack_bitboard[i+extent][j-extent] = 1;
                                if (!input_board[i+extent][j-extent].empty)
                                {
                                    fdv[2] = false;
                                }
                            }
                            else
                            {
                                fdv[2] = false;
                            }

                            if (fdv[3] && valid_index(i-extent, j+extent))
                            {
                                attack_bitboard[i-extent][j+extent] = 1;
                                if (!input_board[i-extent][j+extent].empty)
                                {
                                    fdv[3] = false;
                                }
                            }
                            else
                            {
                                fdv[3] = false;
                            }

                            extent++;
                        }
                        break;
                    case QUEEN:
                        edv = {true, true, true, true,
                               true, true, true, true};
                        extent = 1;
                        while (edv[0] || edv[1] || edv[2] || edv[3] || 
                               edv[4] || edv[5] || edv[6] || edv[7])
                        {
                            if (edv[0] && valid_index(i, j+extent))
                            { 
                                attack_bitboard[i][j+extent] = 1;
                                if (!(input_board[i][j+extent].empty))
                                {
                                    edv[0] = false;
                                }
                            }
                            else
                            {
                                edv[0] = false;
                            }

                            if (edv[1] && valid_index(i, j-extent))
                            {
                                attack_bitboard[i][j-extent] = 1;
                                if (!input_board[i][j-extent].empty)
                                {
                                    edv[1] = false;
                                }
                            }
                            else
                            {
                                edv[1] = false;
                            }

                            if (edv[2] && valid_index(i+extent, j))
                            {
                                attack_bitboard[i+extent][j] = 1;
                                if (!input_board[i+extent][j].empty)
                                {
                                    edv[2] = false;
                                }
                            }
                            else
                            {
                                edv[2] = false;
                            }

                            if (edv[3] && valid_index(i-extent, j))
                            {
                                attack_bitboard[i-extent][j] = 1;
                                if (!input_board[i-extent][j].empty)
                                {
                                    edv[3] = false;
                                }
                            }
                            else
                            {
                                edv[3] = false;
                            }

                            if (edv[4] && valid_index(i+extent, j+extent))
                            { 
                                attack_bitboard[i+extent][j+extent] = 1;
                                if (!(input_board[i+extent][j+extent].empty))
                                {
                                    edv[4] = false;
                                }
                            }
                            else
                            {
                                edv[4] = false;
                            }

                            if (edv[5] && valid_index(i-extent, j-extent))
                            {
                                attack_bitboard[i-extent][j-extent] = 1;
                                if (!input_board[i-extent][j-extent].empty)
                                {
                                    edv[5] = false;
                                }
                            }
                            else
                            {
                                edv[5] = false;
                            }

                            if (edv[6] && valid_index(i+extent, j-extent))
                            {
                                attack_bitboard[i+extent][j-extent] = 1;
                                if (!input_board[i+extent][j-extent].empty)
                                {
                                    edv[6] = false;
                                }
                            }
                            else
                            {
                                edv[6] = false;
                            }

                            if (edv[7] && valid_index(i-extent, j+extent))
                            {
                                attack_bitboard[i-extent][j+extent] = 1;
                                if (!input_board[i-extent][j+extent].empty)
                                {
                                    edv[7] = false;
                                }
                            }
                            else
                            {
                                edv[7] = false;
                            }
                            extent++;
                        }
                        break;
                    case KING:
                        // Front
                        if (valid_index(i+1, j))
                        {
                            attack_bitboard[i+1][j] = 1;
                        }
                        // Back
                        if (valid_index(i-1, j))
                        {
                            attack_bitboard[i-1][j] = 1;
                        }
                        // Right
                        if (valid_index(i, j+1))
                        {
                            attack_bitboard[i][j+1] = 1;
                        }
                        // Left
                        if (valid_index(i, j-1))
                        {
                            attack_bitboard[i][j-1] = 1;
                        }
                        // Forward right
                        if (valid_index(i+1, j+1))
                        {
                            attack_bitboard[i+1][j+1] = 1;
                        } 
                        // Backwards left
                        if (valid_index(i-1, j-1))
                        {
                            attack_bitboard[i-1][j-1] = 1;
                        }
                        // Forward left
                        if (valid_index(i+1, j-1))
                        {
                            attack_bitboard[i+1][j-1] = 1;
                        }
                        // Backwards right
                        if (valid_index(i-1, j+1))
                        {
                            attack_bitboard[i-1][j+1] = 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return attack_bitboard;
}