#include "board.h"
#include <map>

const std::map<int, char> piece_map = {
    {0, 'K'},
    {1, 'P'},
    {2, 'N'},
    {3, 'B'},
    {4, 'R'},
    {5, 'Q'},
};

Board::Board(){
    fillBoard();
}

void Board::fillBoard() {
    
    // board_planes[board][ranks][files]
    // ranks: 0: 1, 1: 2, 2: 3, 3: 4, 4: 5, 5: 6, 6: 7, 7: 8
    // files 0: a, 1: b, 2: c, 3: d, 4: e, 5: f, 6: g, 7: h

    // Locate Kings (white on e1, and black on e8)
    board_planes[0][0][4] = 1;
    board_planes[10][7][4] = 1;
    board_array[0][4] = 'K';
    board_array[7][4] = 'k';

    // Locate Pawns (white on rank 2, and black on rank 7)
    for (int file = 0; file < 8; ++file)
    {
        board_planes[1][1][file] = 1;
        board_planes[11][6][file] = 1;
        board_array[1][file] = 'P';
        board_array[6][file] = 'p';
    }

    // Locate knights (white on b1 and g1, and black on b8 and g8)
    board_planes[2][0][1] = 1;      // white
    board_planes[2][0][6] = 1;      // white
    board_array[0][1] = 'N';        // white
    board_array[0][6] = 'N';        // white

    board_planes[12][7][1] = 1;     // black
    board_planes[12][7][6] = 1;     // black
    board_array[7][1] = 'n';        // black
    board_array[7][6] = 'n';        // black

    // Locate bishops (white on c1 and f1, and black on c8 and f8)
    board_planes[3][0][2] = 1; // white dark
    board_planes[3][0][5] = 1; // white light
    board_array[0][2] = 'B'; // white dark
    board_array[0][5] = 'B'; // white light

    board_planes[13][7][2] = 1; // black light
    board_planes[13][7][5] = 1; // black dark
    board_array[7][2] = 'b'; // black light
    board_array[7][5] = 'b'; // black dark

    // Locate rooks (white on a1 and h1, and black on a8 and h8)
    board_planes[4][0][0] = 1; // white
    board_planes[4][0][7] = 1; // white
    board_array[0][0] = 'R'; // white
    board_array[0][7] = 'R'; // white

    board_planes[14][7][0] = 1; // black 
    board_planes[14][7][7] = 1; // black
    board_array[7][0] = 'r'; // black
    board_array[7][7] = 'r'; // black

    // Locate queens (white on d1, and black on d8)
    board_planes[5][0][3] = 1; // white
    board_array[0][3] = 'Q'; // white

    board_planes[15][7][3] = 1; // black
    board_array[7][3] = 'q'; // black

    // Locate kingside castling rights (default all 0s)
    // Locate queenside castling rights (default all 0s)

    // Fill the rest of the board with empty squares
    for (int rank = 2; rank < 6; ++rank)
    {
        for (int file = 0; file < 8; ++file)
        {
            board_array[rank][file] = ' ';
        }
    }

}

void Board::printBoard() {
    for (int rank = 7; rank >= 0; --rank)
    {
        for (int file = 7; file >= 0; --file)
        {
            std::cout << board_array[rank][file] << " ";
        }
        std::cout << std::endl;
    }
}

