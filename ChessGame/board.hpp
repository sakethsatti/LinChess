#include "bitboard.hpp"

class Board {
private:
    
    /*
        * Represents the entire position using an array of bitboards
        * 0 - White pawns 6 - Black pawns
        * 1 - White knights 7 - Black knights
        * 2 - White bishops 8 - Black bishops
        * 3 - White rooks 9 - Black rooks
        * 4 - White queens 10 - Black queens
        * 5 - White king 11 - Black king
    */
    Bitboard position[12];

    // Game info
    Color turn;
    int halfmove_clock;
    int move_number;

    // Castling rights
    bool white_king_castle;
    bool black_king_castle;
    bool white_queen_castle;
    bool black_queen_castle;


public:
    Board();
    Board(std::string FEN);
    void initialize_position();


};
