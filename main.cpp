#include <iostream>
#include <bitset>
#include "ChessGame/board.hpp"

int main() {
    Board board = Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    // print_bitboard(board.findPinnedPieces());
    
    // std::cout << board.turn << std::endl;
    // std::cout << board.halfmove_clock << std::endl;
    // std::cout << board.move_number << std::endl;
    // std::cout << board.white_king_castle << std::endl;
    // std::cout << board.black_king_castle << std::endl;
    // std::cout << board.white_queen_castle << std::endl;
    // std::cout << board.black_queen_castle << std::endl;
    // std::cout << board.en_passant_square << std::endl;


    board.print_position();
    auto legalMoves = board.genLegalMoves();

    std::cout << legalMoves.size() << std::endl;

    array<std::string, 64> chess_squares = {
        "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
        "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
        "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
        "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
        "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
        "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
        "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
    };



    for (const auto& move : legalMoves) {
        if (move.king_castle) {
            std::cout << "O-O" << std::endl;
        } else if (move.queen_castle) {
            std::cout << "O-O-O" << std::endl;
        } else {
            std::cout << chess_squares[move.from] << chess_squares[move.to] << std::endl;
        }
        
    }
    
    return 0;
}