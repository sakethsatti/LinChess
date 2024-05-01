#include "board.h"
#include <map>

Board::Board(){
    fill_board();
}

void Board::fill_board() {
    
    // Instance Variables
    player_king_moved = false;
    opp_king_moved = false;
    player_rook_kingside_moved = false;
    player_rook_queenside_moved = false;
    in_check = false;
    opp_rook_kingside_moved = false;
    opp_rook_queenside_moved = false;
    moves_since_last_capture = 0;
    moves_since_last_pawn_move = 0;

    turn = 'w';

    // board_planes[board][ranks][files]
    // ranks: 0: 1, 1: 2, 2: 3, 3: 4, 4: 5, 5: 6, 6: 7, 7: 8
    // files 0: a, 1: b, 2: c, 3: d, 4: e, 5: f, 6: g, 7: h

    // Locate Kings (white on e1, and black on e8)
    board_array[0][4] = 'K';
    board_array[7][4] = 'k';

    // Locate Pawns (white on rank 2, and black on rank 7)
    for (int file = 0; file < 8; ++file)
    {
        board_array[1][file] = 'P';
        board_array[6][file] = 'p';
    }

    // Locate knights (white on b1 and g1, and black on b8 and g8)
    board_array[0][1] = 'N';        // white
    board_array[0][6] = 'N';        // white
    board_array[7][1] = 'n';        // black
    board_array[7][6] = 'n';        // black

    // Locate bishops (white on c1 and f1, and black on c8 and f8)
    board_array[0][2] = 'B'; // white dark
    board_array[0][5] = 'B'; // white light
    board_array[7][2] = 'b'; // black light
    board_array[7][5] = 'b'; // black dark

    // Locate rooks (white on a1 and h1, and black on a8 and h8)
    board_array[0][0] = 'R'; // white
    board_array[0][7] = 'R'; // white
    board_array[7][0] = 'r'; // black
    board_array[7][7] = 'r'; // black

    // Locate queens (white on d1, and black on d8)
    board_array[0][3] = 'Q'; // white
    board_array[7][3] = 'q'; // black

    // Fill the rest of the board with empty squares
    for (int rank = 2; rank < 6; ++rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            board_array[rank][file] = ' ';
        }
    }

}

void Board::print_board() {
    for (int rank = 7; rank >= 0; --rank)
    {
        for (int file = 7; file >= 0; --file)
        {
            std::cout << board_array[rank][file] << " ";
        }
        std::cout << std::endl;
    }
}