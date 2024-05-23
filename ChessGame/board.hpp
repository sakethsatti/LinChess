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


public:
    Board();
    Board(std::string FEN);
    void initialize_position();


};
