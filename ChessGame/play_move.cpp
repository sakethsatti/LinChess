#include "board.hpp"
#include <stdexcept>
#include <iostream>

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

    switch(board_repr[old_file][old_rank].piece.type) {
        case PAWN:
            if (!valid_pawn_move(old_file, old_rank, new_file, new_rank)) {
                throw std::invalid_argument("Invalid piece movement");
            }
            break;
        case KNIGHT:
            if (!valid_knight_move(old_file, old_rank, new_file, new_rank)) {
                throw std::invalid_argument("Invalid piece movement");
            } 
            break;
        case BISHOP:
            if (!valid_bishop_move(old_file, old_rank, new_file, new_rank)) {
                throw std::invalid_argument("Invalid piece movement");
            }
            break;

        case ROOK:
            std::cout << "Rook move\n";
            if (!valid_rook_move(old_file, old_rank, new_file, new_rank)) {
                throw std::invalid_argument("Invalid piece movement");
            }
            break;
        case QUEEN:
            std::cout << "Queen move\n";
            // !(legal_bishop_move || legal_rook_move) = !legal_bishop_move && !legal_rook_move
            if (!valid_bishop_move(old_file, old_rank, new_file, new_rank) &&
                !valid_rook_move(old_file, old_rank, new_file, new_rank))
            {
                throw std::invalid_argument("Invalid piece movement");
            }
            break;
        case KING:
            std::cout << "King move\n";
            if (!valid_king_move(old_file, old_rank, new_file, new_rank))
            {
                throw std::invalid_argument("Invalid piece movement");
            } 
            break;

        default:
            break;
    }

    SquareArray board_copy = board_repr;
    board_copy[new_file][new_rank] = board_copy[old_file][old_rank];
    board_copy[old_file][old_rank] = Square(EMPTY, NONE);

    if (is_in_check(board_copy)) {
        throw std::invalid_argument("Invalid move: leads to king being taken");
    }

    // Move piece
    board_repr[new_file][new_rank] = board_repr[old_file][old_rank];
    board_repr[old_file][old_rank] = Square(EMPTY, NONE);

    // Change turn
    turn = (turn == WHITE) ? BLACK : WHITE;

    print_board();
}


/*
    * Function to check if the player is in check
    * Notice the use of board param rather than board_repr
    * This will be useful when checking for a pin.
*/
bool Board::is_in_check(SquareArray& board) {
    
    BitBoardAttack current_opp_attacks = generate_attack_bitboards(board, Piece::get_opp_color(turn));

    for (int i = 0; i < FILES; i++) {
        for (int j = 0; j < RANKS; j++) {
            if (board[i][j].piece.type == KING &&
                board[i][j].piece.color == turn)
            {
                if (current_opp_attacks[i][j] == 1)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

// Check if valid pawn move
bool Board::valid_pawn_move(int old_file, int old_rank, int new_file, int new_rank) {
    if (turn == WHITE) {
        if (new_rank == old_rank + 1 && new_file == old_file && board_repr[new_file][new_rank].empty) {
            return true;
        } else if (new_rank == old_rank + 2 && new_file == old_file && old_rank == 1) {
            return true;
        } else if (new_rank == old_rank + 1 && (new_file == old_file + 1 || new_file == old_file - 1)) {
            return true;
        } else {
            return false;
        }
    } else {
        if (new_rank == old_rank - 1 && new_file == old_file && board_repr[new_file][new_rank].empty) {
            return true;
        } else if (new_rank == old_rank - 2 && new_file == old_file && old_rank == 6) {
            return true;
        } else if (new_rank == old_rank - 1 && (new_file == old_file + 1 || new_file == old_file - 1)) {
            return true;
        } else {
            return false;
        }
    }
}

// Check if valid knight move
bool Board::valid_knight_move(int old_file, int old_rank, int new_file, int new_rank) {
    int file_diff = abs(new_file - old_file);
    int rank_diff = abs(new_rank - old_rank);

    if ((file_diff == 2 && rank_diff == 1) || (file_diff == 1 && rank_diff == 2)) {
        return true;
    } 
    
    return false;
}

// Check if valid bishop move
bool Board::valid_bishop_move(int old_file, int old_rank, int new_file, int new_rank) {
    if (abs(new_file - old_file) != abs(new_rank - old_rank)) {
        return false;
    }

    if (new_file > old_file && new_rank > old_rank) {
        for (int i = 1; i < abs(new_file - old_file); i++) {
            if (!board_repr[old_file + i][old_rank + i].empty) {
                return false;
            }
        }
    } else if (new_file > old_file && new_rank < old_rank) {
        for (int i = 1; i < abs(new_file - old_file); i++) {
            if (!board_repr[old_file + i][old_rank - i].empty) {
                return false;
            }
        }
    } else if (new_file < old_file && new_rank > old_rank) {
        for (int i = 1; i < abs(new_file - old_file); i++) {
            if (!board_repr[old_file - i][old_rank + i].empty) {
                return false;
            }
        }
    } else {
        for (int i = 1; i < abs(new_file - old_file); i++) {
            if (!board_repr[old_file - i][old_rank - i].empty) {
                return false;
            }
        }
    }

    return true;
}

// Check if valid rook move
bool Board::valid_rook_move(int old_file, int old_rank, int new_file, int new_rank) {
    if (new_file != old_file && new_rank != old_rank) {
        return false;
    }

    if (new_file == old_file) {
        if (new_rank > old_rank) {
            for (int i = old_rank + 1; i < new_rank; i++) {
                if (!board_repr[old_file][i].empty) {
                    return false;
                }
            }
        } else {
            for (int i = old_rank - 1; i > new_rank; i--) {
                if (!board_repr[old_file][i].empty) {
                    return false;
                }
            }
        }
    } else {
        if (new_file > old_file) {
            for (int i = old_file + 1; i < new_file; i++) {
                if (!board_repr[i][old_rank].empty) {
                    return false;
                }
            }
        } else {
            for (int i = old_file - 1; i > new_file; i--) {
                if (!board_repr[i][old_rank].empty) {
                    return false;
                }
            }
        }
    }

    return true;
}

// Check if valid king move
bool Board::valid_king_move(int old_file, int old_rank, int new_file, int new_rank) {
    int file_diff = abs(new_file - old_file);
    int rank_diff = abs(new_rank - old_rank);

    if (file_diff <= 1 && rank_diff <= 1) {
        return true;
    }

    return false;
}