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

    en_passant_square = SQ_NONE;

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
        en_passant_square = (Pos)(FEN_parts[3][0] - 'a' + 8 * (FEN_parts[3][1] - '1'));
    } else {
        en_passant_square = SQ_NONE;
    }

    halfmove_clock = (FEN_parts[4] != "") ? std::stoi(FEN_parts[4]) : 0;
    move_number = (FEN_parts[5] != "") ? std::stoi(FEN_parts[5]) : 0;

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

// color is the color of the king we are checking for
Bitboard Board::findUnsafeKingSquares(Color color)
{

    Bitboard attacks = 0ULL;
    // Here we must ignore the king to find squares that will be unsafe after the king has moved 
    Bitboard friendPieces = position[6 + color] & ~(position[KING]);
    Bitboard& oppPieces = position[7 - color];

    // Notice the use of 6 + color get the opposite color's pieces 
    for (int i = 0; i < 64; ++i)
    {
         
        if (oppPieces & (1ULL << i))
        {
            if (position[PAWN] & oppPieces & (1ULL << i))
            {
                attacks |= pawnAttacks((Pos)i, Color(1 - color));
            }
            else if (position[KNIGHT] & oppPieces & (1ULL << i))
            {
                attacks |= knightAttacks((Pos)i);
            }
            else if (position[BISHOP] & oppPieces & (1ULL << i))
            {
                attacks |= bishopAttacks((Pos)i, friendPieces | oppPieces);
            }
            else if (position[ROOK] & oppPieces & (1ULL << i))
            {
                attacks |= rookAttacks((Pos)i, friendPieces | oppPieces);
            }
            else if (position[QUEEN] & oppPieces & (1ULL << i))
            {
                attacks |= queenAttacks((Pos)i, friendPieces | oppPieces);
            }
            else if (position[KING] & oppPieces & (1ULL << i))
            {
                attacks |= kingAttacks((Pos)i);
            }
        
        }
    }

    return attacks;
}

bool Board::in_check()
{
    Bitboard attacks = findUnsafeKingSquares(turn);
    // attacks & (active king)
    return attacks & (position[KING] & position[6 + turn]);
}

vector<pair<Piece, Bitboard>> Board::findKingAttackers()
{
    vector<pair<Piece, Bitboard>> attackers;

    if (!in_check()) return attackers;

    Pos kingPos = (Pos)calcLSB(position[6 + turn] & position[5]);
    Bitboard& theOpps = position[7 - turn];

    // Find all pieces that seem to be attacking the king
    // King cannot directly attack the king so we don't need to check for that
    Bitboard pawnAttackers = pawnAttacks(kingPos, turn) & position[0] & theOpps & position[0];
    Bitboard knightAttackers = knightAttacks(kingPos) & position[1] & theOpps & position[1];
    Bitboard bishopAttackers = bishopAttacks(kingPos, position[6] | position[7]) & position[2] & theOpps & position[2];
    Bitboard RookAttackers = rookAttacks(kingPos, position[6] | position[7]) & position[3] & theOpps & position[3];

    // There can be more than one queen attacking the king possibly in a game
    Bitboard queenAttackers = queenAttacks(kingPos, position[6] | position[7]) & position[4] & theOpps & position[4];

    if (pawnAttackers)
    {
        attackers.push_back({PAWN, pawnAttackers});
    }

    if (knightAttackers)
    {
        attackers.push_back({KNIGHT, knightAttackers});
    }

    if (bishopAttackers)
    {
        attackers.push_back({BISHOP, bishopAttackers});
    }

    if (RookAttackers)
    {
        attackers.push_back({ROOK, RookAttackers});
    }

    while (queenAttackers)
    {
        attackers.push_back({QUEEN, queenAttackers & (1ULL << calcLSB(queenAttackers))});
        queenAttackers -= 1ULL << calcLSB(queenAttackers);
    }

    return attackers;
}

Bitboard Board::findPinnedPieces()
{
    Bitboard pinnedPieces = 0ULL;

    // Get all the opposition sliders to iterate through
    Bitboard bishopOpps = position[7 - turn] & position[BISHOP];
    Bitboard rookOpps = position[7 - turn] & position[ROOK];
    Bitboard queenOpps = position[7 - turn] & position[QUEEN];

    // Get king position
    Pos kingPos = (Pos)calcLSB(position[KING] & position[6 + turn]);

    // Get rays as if there was a opp in kings position
    Bitboard bishopRays = bishopAttacks(kingPos, position[6] | position[7]) & position[6 + turn];
    Bitboard rookRays = rookAttacks(kingPos, position[6] | position[7]) & position[6 + turn];

    // Possible squares for a piece creating a pin
    Bitboard posBishopPinner = calcBishopMask(kingPos, true);
    Bitboard posRookPinner = calcRookMask(kingPos, true);

    bishopOpps &= posBishopPinner;
    rookOpps &= posRookPinner;
    queenOpps &= posRookPinner | posBishopPinner;

    // Bishops
    while (bishopOpps)
    {

        Pos nextBishop = (Pos)calcLSB(bishopOpps);
        Bitboard nextBishopAttacks = bishopAttacks(nextBishop, position[6] | position[7]) & position[6 + turn];

        pinnedPieces |= nextBishopAttacks & bishopRays;

        bishopOpps -= 1ULL << nextBishop;
    }

    // Rooks
    while (rookOpps)
    {
        Pos nextRook = (Pos)calcLSB(rookOpps);
        Bitboard nextRookAttacks = rookAttacks(nextRook, position[6] | position[7]) & position[6 + turn];

        pinnedPieces |= nextRookAttacks & rookRays;

        rookOpps -= 1ULL << nextRook;
    }

    // Queen
    while (queenOpps)
    {
        Pos nextQueen = (Pos)calcLSB(queenOpps);
        Bitboard nextRookComponent = rookAttacks(nextQueen, position[6] | position[7]) & position[6 + turn];
        Bitboard nextBishopComponent = bishopAttacks(nextQueen, position[6] | position[7]) & position[6 + turn];

        pinnedPieces |= nextRookComponent & rookRays & posRookPinner;
        pinnedPieces |= nextBishopComponent & bishopRays & posBishopPinner;

        queenOpps -= 1ULL << nextQueen;
    }

    return pinnedPieces;
}

Bitboard Board::attacksBySliders(Bitboard bishops, Bitboard rooks, Bitboard queens, Bitboard allPieces)
{
    Bitboard attacks = 0ULL;

    while (rooks)
    {
        Pos nextRook = (Pos)calcLSB(rooks);
        attacks |= rookAttacks(nextRook, allPieces);
        rooks -= 1ULL << nextRook;
    }

    while (bishops)
    {
        Pos nextBishop = (Pos)calcLSB(bishops);
        attacks |= bishopAttacks(nextBishop, allPieces);
        bishops -= 1ULL << nextBishop;
    }

    while (queens)
    {
        Pos nextQueen = (Pos)calcLSB(queens);
        attacks |= queenAttacks(nextQueen, allPieces);
        queens -= 1ULL << nextQueen;
    }

    return attacks;
}

LegalMoves Board::genLegalMoves()
{
    Bitboard allColorPieces;
    Bitboard allOppPieces;

    bool is_in_check = in_check();

    if (turn == WHITE)
    {
        allColorPieces = position[6];
        allOppPieces = position[7];
    }
    else {
        allColorPieces = position[7];
        allOppPieces = position[6];
    }
    LegalMoves legal_moves;

    // print_bitboard(allColorPieces);
    // print_bitboard(allOppPieces);

    Bitboard pinnedPiecesMask = findPinnedPieces();

    // Useful for detecting kings walking into checks
    Bitboard kingDanger = findUnsafeKingSquares(turn);
    Bitboard en_passant_mask = (en_passant_square != SQ_NONE) ? 1ULL << en_passant_square : 0ULL;

    vector<pair<Piece, Bitboard>> attackers = findKingAttackers();

    // std::cout << attackers.size() << std::endl;

    for (int i = 0; i < 64; ++i)
    {
        Piece pieceMoving;
        Bitboard allPieceMoves = 0ULL;

        // Check if absolute pin (can't move at all);
        if ((1ULL << i) & pinnedPiecesMask)
        {
            continue;
        }

        // If the piece isn't a king and there are multiple attackers, only the king can move
        if (attackers.size() > 1 && !(allColorPieces & position[KING] & (1ULL << i)))
        {
            continue;
        }

        if (allColorPieces & position[KING] & (1ULL << i)) {
            allPieceMoves = kingAttacks((Pos)i) & ~kingDanger & ~allColorPieces;
            pieceMoving = KING;
        } else if (allColorPieces & position[PAWN] & (1ULL << i)) {
            allPieceMoves = pawnMoves((Pos)i, allColorPieces | allOppPieces, turn);

            Bitboard attackingSquares = pawnAttacks((Pos)i, turn);

            allPieceMoves |= attackingSquares & allOppPieces;

            // Check for safe en_passant
            // This is a relatively expensive calculation, but its worth it because en_passant is quite rare
            if (attackingSquares & en_passant_mask)
            {
                // Recalculate position where en_passant is played
                Bitboard newPosition = (position[6] | position[7] | (1ULL << en_passant_square)) & // Combine all pieces bitboard with en_passant square (where the capturing pawn will land)
                                       ~(
                                            (1ULL << i) | // Remove current square
                                            ((turn == WHITE) ? (1ULL << (en_passant_square - 8)) : (1ULL << (en_passant_square + 8))) // Remove pawn captured by en_passant
                                        );
                

                Bitboard newAttacked = attacksBySliders(position[2] & position[7 - turn], position[3] & position[7 - turn], position[4] & position[7 - turn], newPosition);

                if (!(newAttacked & (position[5] & position[6 + turn])))
                {
                    allPieceMoves |= en_passant_mask;
                }
            }

            pieceMoving = PAWN;
        } else if (allColorPieces & position[KNIGHT] & (1ULL << i)) {
            allPieceMoves = knightAttacks((Pos)i) & ~allColorPieces;
            pieceMoving = KNIGHT;
        } else if (allColorPieces & position[BISHOP] & (1ULL << i)) {
            allPieceMoves = bishopAttacks((Pos)i, allColorPieces | allOppPieces) & ~allColorPieces;
            pieceMoving = BISHOP;
        } else if (allColorPieces & position[ROOK] & (1ULL << i)) {
            allPieceMoves = rookAttacks((Pos)i, allColorPieces | allOppPieces) & ~allColorPieces;
            pieceMoving = ROOK;
        } else if (allColorPieces & position[QUEEN] & (1ULL << i)) {
            allPieceMoves = queenAttacks((Pos)i, allColorPieces | allOppPieces) & ~allColorPieces;
            pieceMoving = QUEEN;
        } else {
            continue;
        }

        if (attackers.size() == 1 && pieceMoving != KING)
        {
            // Capture piece
            Bitboard possibleCheckDefenseMask = attackers[0].second;

            if (pieceMoving == PAWN)
            {
                possibleCheckDefenseMask &= pawnAttacks((Pos)i, turn);
            }

            

            possibleCheckDefenseMask = possibleCheckDefenseMask | 
                                        mask_between(calcLSB(position[5] & position[6 + turn]),
                                                        calcLSB(attackers[0].second));
            
            if (i == 0)
            {
                print_bitboard(possibleCheckDefenseMask);
            }

            allPieceMoves &= possibleCheckDefenseMask;
        }
 
        while (allPieceMoves)
        {

            int moveSquare = calcLSB(allPieceMoves);
            Pos enPassantSquare = SQ_NONE;
            if (pieceMoving == PAWN && abs(moveSquare - i) == 16)
            {
                enPassantSquare = (Pos)((i + moveSquare) / 2);
            }

            if (pieceMoving == PAWN && ((allPieceMoves) & (RANK_1 | RANK_8)))
            {
                legal_moves.push_back(
                    Move((Pos)i, (Pos)moveSquare, turn, false, false, enPassantSquare, KNIGHT)
                );

                legal_moves.push_back(
                    Move((Pos)i, (Pos)moveSquare, turn, false, false, enPassantSquare, BISHOP)
                );

                legal_moves.push_back(
                    Move((Pos)i, (Pos)moveSquare, turn, false, false, enPassantSquare, ROOK)
                );

                legal_moves.push_back(
                    Move((Pos)i, (Pos)moveSquare, turn, false, false, enPassantSquare, QUEEN)
                );
            } else {
                legal_moves.push_back(
                    Move((Pos)i, (Pos)moveSquare, turn, false, false, enPassantSquare, NONE)
                );
            }

            allPieceMoves -= (1ULL << moveSquare);
        }
    }

    if(attackers.size() == 0)
    {
        if (turn == WHITE)
        {
            if (white_king_castle && !(kingDanger & (1ULL << 5)) && !(allColorPieces & (1ULL << 5)) && !(allOppPieces & (1ULL << 5)) &&
                !(kingDanger & (1ULL << 6)) && !(allColorPieces & (1ULL << 6)) && !(allOppPieces & (1ULL << 6)))
            {
                legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, true, false, SQ_NONE, NONE));
            }

            
            if (white_queen_castle &&
                !(kingDanger & (1ULL << 1)) && !(allColorPieces & (1ULL << 1)) && !(allOppPieces & (1ULL << 1)) &&
                !(kingDanger & (1ULL << 2)) && !(allColorPieces & (1ULL << 2)) && !(allOppPieces & (1ULL << 2)) &&
                !(kingDanger & (1ULL << 3)) && !(allColorPieces & (1ULL << 3)) && !(allOppPieces & (1ULL << 3)))
            {
                legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, false, true, SQ_NONE, NONE));
            }
        }
        else
        {
            if (black_king_castle && !(kingDanger & (1ULL << 61)) && !(allColorPieces & (1ULL << 61)) && !(allOppPieces & (1ULL << 61)) &&
                !(kingDanger & (1ULL << 62)) && !(allColorPieces & (1ULL << 62)) && !(allOppPieces & (1ULL << 62)))
            {
                legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, true, false, SQ_NONE, NONE));
            }
            if (black_queen_castle &&
                !(kingDanger & (1ULL << 57)) && !(allColorPieces & (1ULL << 57)) && !(allOppPieces & (1ULL << 57)) &&
                !(kingDanger & (1ULL << 58)) && !(allColorPieces & (1ULL << 58)) && !(allOppPieces & (1ULL << 58)) &&
                !(kingDanger & (1ULL << 59)) && !(allColorPieces & (1ULL << 59)) && !(allOppPieces & (1ULL << 59)))
            {
                legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, false, true, SQ_NONE, NONE));
            }
        }
    }

    return legal_moves;

}