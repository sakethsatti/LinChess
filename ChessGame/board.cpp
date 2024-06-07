#include "board.hpp"
#include <sstream>

Board::Board() {
    
    // Initialize position
    position[0] = RANK_2 | RANK_7;
    position[1] = (1ULL << 1) | (1ULL << 6) | (1ULL << 57) | (1ULL << 62);
    position[2] = (1ULL << 2) | (1ULL << 5) | (1ULL << 58) | (1ULL << 61);
    position[3] = (1ULL << 0) | (1ULL << 7) | (1ULL << 56) | (1ULL << 63);
    position[4] = (1ULL << 3) | (1ULL << 59);
    position[5] = (1ULL << 4) | (1ULL << 60);
    position[6] = RANK_2 | RANK_1;
    position[7] = RANK_7 | RANK_8;

    // Initialize game info
    turn = WHITE;
    halfmove_clock = 0;
    move_number =  1;

    en_passant_square = -1;

    white_king_castle = true;
    black_king_castle = true;
    white_queen_castle = true;
    black_queen_castle = true;


}

Board::Board(std::string FEN) {
    
    // 0 - board 1 - turn 2 - castling 3 - en passant 4 - halfmove clock 5 - fullmove number
    array<string, 6> FEN_parts;
    std::stringstream ss(FEN);
    vector<string> v;
    string temp;

    int i = 0;
    while (std::getline(ss, temp, ' ')) {
 
        // store token string in the vector
        FEN_parts[i] = temp;
        std::cout << FEN_parts[i] << std::endl;
        ++i;
    }

    position = {0};

    int fen_square = 0;
    // Initialize position
    for (char piece : FEN_parts[0])
    {
        if (std::isalpha(piece)) {
            switch (piece) {
                case 'P':
                    position[0] |= 1ULL << FEN_TO_POS[fen_square];
                    position[6] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'p':
                    position[0] |= 1ULL << FEN_TO_POS[fen_square];
                    position[7] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'N':
                    position[1] |= 1ULL << FEN_TO_POS[fen_square];
                    position[6] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'n':
                    position[1] |= 1ULL << FEN_TO_POS[fen_square];
                    position[7] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'B':
                    position[2] |= 1ULL << FEN_TO_POS[fen_square];
                    position[6] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'b':
                    position[2] |= 1ULL << FEN_TO_POS[fen_square];
                    position[7] |= 1ULL << FEN_TO_POS[fen_square];
                    std::cout << FEN_TO_POS[fen_square] << std::endl;
                    break;
                case 'R':
                    position[3] |= 1ULL << FEN_TO_POS[fen_square];
                    position[6] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'r':
                    position[3] |= 1ULL << FEN_TO_POS[fen_square];
                    position[7] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'Q':
                    position[4] |= 1ULL << FEN_TO_POS[fen_square];
                    position[6] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'q':
                    position[4] |= 1ULL << FEN_TO_POS[fen_square];
                    position[7] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'K':
                    position[5] |= 1ULL << FEN_TO_POS[fen_square];
                    position[6] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                case 'k':
                    position[5] |= 1ULL << FEN_TO_POS[fen_square];
                    position[7] |= 1ULL << FEN_TO_POS[fen_square];
                    break;
                default:
                    std::cout << "what" << std::endl;
                    break;
            }
            ++fen_square;
        } else if (std::isdigit(piece)) {
            fen_square += piece - '0';
        } else if (piece == '/') {
            continue;
        }
    }

    if (FEN_parts[1] == "w") {
        turn = WHITE;
    } else {
        turn = BLACK;
    }

    white_king_castle = false;
    black_king_castle = false;
    white_queen_castle = false;
    black_queen_castle = false;

    for (char c : FEN_parts[2]) {
        switch (c) {
            case 'K':
                white_king_castle = true;
                break;
            case 'Q':
                white_queen_castle = true;
                break;
            case 'k':
                black_king_castle = true;
                break;
            case 'q':
                black_queen_castle = true;
                break;
            default:
                break;
        }
    }

    if (FEN_parts[3] != "-") {
        en_passant_square = FEN_parts[3][0] - 'a' + 8 * (FEN_parts[3][1] - '1');
    } else {
        en_passant_square = -1;
    }

    halfmove_clock = std::stoi(FEN_parts[4]);
    move_number = std::stoi(FEN_parts[5]);

}

void Board::print_position() {
    for (int i = 7; i >= 0 ; --i)
    {
        for (int j = 0; j < 8; ++j)
        {
            
            if (position[0] & position[7] & (1ULL << (8 * i + j)))
            {
                std::cout << "♙ ";
            }
            else if (position[1] & position[7] & (1ULL << (8 * i + j)))
            {
                std::cout << "♘ ";
            }
            else if (position[2] & position[7] & (1ULL << (8 * i + j)))
            {
                std::cout << "♗ ";
            }
            else if (position[3] & position[7] & (1ULL << (8 * i + j)))
            {
                std::cout << "♖ ";
            }
            else if (position[4] & position[7] & (1ULL << (8 * i + j)))
            {
                std::cout << "♕ ";
            }
            else if (position[5] & position[7] & (1ULL << (8 * i + j)))
            {
                std::cout << "♔ ";
            }
            else if (position[0] & position[6] & (1ULL << (8 * i + j)))
            {
                std::cout << "♟ ";
            }
            else if (position[1] & position[6] & (1ULL << (8 * i + j)))
            {
                std::cout << "♞ ";
            }
            else if (position[2] & position[6] & (1ULL << (8 * i + j)))
            {
                std::cout << "♝ ";
            }
            else if (position[3] & position[6] & (1ULL << (8 * i + j)))
            {
                std::cout << "♜ ";
            }
            else if (position[4] & position[6] & (1ULL << (8 * i + j)))
            {
                std::cout << "♛ ";
            }
            else if (position[5] & position[6] & (1ULL << (8 * i + j)))
            {
                std::cout << "♚ ";
            }
            else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}