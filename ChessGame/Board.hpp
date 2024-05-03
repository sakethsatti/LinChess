#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <array>
#include <string>

using std::vector;
using std::array;
using std::string;

// Piece types and piece colors as enumerations
enum PieceTypes { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum Color { WHITE, BLACK };

// Struct piece containing a piece type and a piece color
struct Piece {
    PieceTypes type;
    Color color;
};

// Struct square containing a piece and a boolean indicating if the square is empty
struct Square {
    Piece piece;
    bool empty;
};

/* 
    * Define board shape
    * Creates a 8 x 8 array of Squares
    * Rows are files and columns are ranks
    * (0, 0) is the bottom left corner (a1)
    * (7, 7) is the top right corner (h8)
*/
#define FILES 8
#define RANKS 8
typedef array<array<Piece, RANKS>, FILES> SquareArray; 

class Board {
private:
    
    // Internal board representation
    SquareArray board_repr;

    // Extra details
    int halfmove_clock;
    Color turn;
    bool white_king_castle;
    bool white_queen_castle;
    bool black_king_castle;
    bool black_queen_castle;
    

    // Helper functions
    bool is_in_check(SquareArray board);
    bool is_in_checkmate(SquareArray board);
    bool is_in_stalemate(SquareArray board);

public:
    Board();
    Board(string fen);

    void init_board();
    void print_board();
    void move_piece(int old_file, int old_rank, int new_file, int new_rank);
    vector<array<int, 4>> generate_legal_moves();

    array<int, 81920> get_nn_input();

};

#endif // BOARD_HPP