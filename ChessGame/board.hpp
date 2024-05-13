#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <array>
#include <string>
#include <map>

using std::vector;
using std::array;
using std::string;
using std::map;

// Piece types and piece colors as enumerations
enum PieceTypes { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY };
enum Color { WHITE, BLACK, NONE };

// Struct piece containing a piece type and a piece color
struct Piece {
    PieceTypes type;
    Color color;

    Piece(PieceTypes piecetype, Color piececolor) : type(piecetype), color(piececolor) {}
};

// Struct square containing a piece and a boolean indicating if the square is empty
struct Square {
    Piece piece = Piece(EMPTY, NONE);
    bool empty;

    Square() : empty(true), piece(Piece(EMPTY, NONE)) {}
    Square(PieceTypes piecetype, Color color) : empty(false), piece(Piece(piecetype, color)) {}
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
typedef array<array<Square, RANKS>, FILES> SquareArray; 

class Board {
private:
    
    // Internal board representation
    SquareArray board_repr;
    const std::map<int, char> PIECE_TO_CHAR {
        {PAWN, 'P'},
        {ROOK, 'R'},
        {KNIGHT, 'N'},
        {BISHOP, 'B'},
        {QUEEN, 'Q'},
        {KING, 'K'},
        {EMPTY, '.'}
    };

    array<array<int, RANKS>, FILES> white_attack_bitboard;
    array<array<int, RANKS>, FILES> black_attack_bitboard;

    // Bitboard activities
    void generate_attack_bitboards();

    // Game state functions and variables
    bool is_in_check(const SquareArray& board);
    bool is_in_checkmate(const SquareArray& board);
    bool is_in_stalemate(const SquareArray& board);
    
    int halfmove_clock;
    Color turn;
    bool white_king_castle;
    bool white_queen_castle;
    bool black_king_castle;
    bool black_queen_castle;
    

    // Misc functions
    bool valid_index(int file, int rank);

public:
    Board();
    Board(string fen);

    void print_attack_bitboards();
    void print_board();
    void move_piece(int old_file, int old_rank, int new_file, int new_rank);
    vector<array<int, 4>> generate_legal_moves();

    array<int, 81920> get_nn_input();

};

#endif // BOARD_HPP