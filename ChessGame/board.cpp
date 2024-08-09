#include "board.hpp"
#include "bitboard.hpp"
#include "constants.hpp"
#include <sstream>
#include <stdexcept>

Board::Board(): orgGameState(Move(SQ_NONE, SQ_NONE, WHITE, false, false, SQ_NONE, NONE))
{

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

Board::Board(std::string FEN): orgGameState(Move(SQ_NONE, SQ_NONE, WHITE, false, false, SQ_NONE, NONE))

{

  // 0 - board 1 - turn 2 - castling 3 - en passant 4 - halfmove clock 5 - fullmove number
  array<string, 6> FEN_parts;
  std::stringstream ss(FEN);
  vector<string> v;
  string temp;

  int i = 0;
  while (std::getline(ss, temp, ' ')) {
    if (i > 5) {
      // Avoid extra spaces causing a segfault
      break;
    }

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
 
  orgGameState = Move(SQ_NONE, SQ_NONE, turn, false, false, 
                      en_passant_square, NONE, white_king_castle,
                      white_queen_castle, black_king_castle, black_queen_castle);
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
// turn is the color of the king we are checking for
Bitboard Board::findUnsafeKingSquares(Color turn)
{

  Bitboard attacks = 0ULL;
  // Here we must ignore the king to find squares that will be unsafe after the king has moved 
  Bitboard friendPieces = position[6 + turn] & ~(position[KING]);
  Bitboard oppPieces = position[7 - turn];

  // Notice the use of 6 + turn get the opposite color's pieces 
  for (int i = 0; i < 64; ++i)
  {

    if (oppPieces & (1ULL << i))
    {
      if (position[PAWN] & oppPieces & (1ULL << i))
      {
        attacks |= pawnAttacks((Pos)i, Color(1 - turn));
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

bool Board::equals(const Board& b) {
    // Compare bitboards
    if (position != b.position) 
    {
      std::cout << "position !=" << std::endl;
      return false;
    }

    // Compare game info
    if (turn != b.turn) {
        
      std::cout << "turn" << std::endl;
      return false;
    }

    // Compare castling rights
    if (white_king_castle != b.white_king_castle ||
        black_king_castle != b.black_king_castle ||
        white_queen_castle != b.white_queen_castle ||
        black_queen_castle != b.black_queen_castle) {
      std::cout << "castle" << std::endl;
      return false;
    }

    // Compare en passant square
    if (en_passant_square != b.en_passant_square) {
       
      std::cout << "en_passant" << std::endl;
      return false;
    }

    if (movesList.size() != b.movesList.size()) {
        
      std::cout << "movesList" << std::endl;
      return false;
    }

    // If all comparisons pass, the boards are equal
    return true;
}

void Board::moveMaker(const Move& move)
{ 
  // Remove in case of captures
  if (move.capture != NONE && move.en_passant_taken == SQ_NONE)
  {
    position[move.capture] ^= 1ULL << move.to; 
    position[7 - move.color] &= ~(1ULL << move.to);
  } 
  else if (move.capture != NONE && move.en_passant_taken < SQ_NONE)
  {
    if (move.color == WHITE)
    {
      position[PAWN] ^= (1ULL << (move.to - 8));
      position[7] ^= (1ULL << (move.to - 8));
    } else {
      position[PAWN] ^= (1ULL << (move.to + 8));
      position[6] ^= (1ULL << (move.to + 8));
    }
  }

  if (move.kc && move.color == WHITE)
  {
    position[ROOK] |= (1ULL << 5);
    position[KING] |= (1ULL << 6);
    position[6] |= (1ULL << 5);
    position[6] |= (1ULL << 6);

    position[ROOK] &= ~(1ULL << 7);
    position[KING] &= ~(1ULL << 4);
    position[6] &= ~(1ULL << 4);
    position[6] &= ~(1ULL << 7);

  } else if (move.qc && move.color == WHITE)
  {
    position[ROOK] |= (1ULL << 3);
    position[KING] |= (1ULL << 2);
    position[6] |= (1ULL << 3);
    position[6] |= (1ULL << 2);

    position[ROOK] &= ~(1ULL << 0);
    position[KING] &= ~(1ULL << 4);
    position[6] &= ~(1ULL << 4);
    position[6] &= ~(1ULL << 0);
  } else if (move.kc && move.color == BLACK)
  {
    position[ROOK] |= (1ULL << 61);
    position[KING] |= (1ULL << 62);
    position[7] |= (1ULL << 61);
    position[7] |= (1ULL << 62);

    position[ROOK] &= ~(1ULL << 63);
    position[KING] &= ~(1ULL << 60);
    position[7] &= ~(1ULL << 60);
    position[7] &= ~(1ULL << 63);
  } else if (move.qc && move.color == BLACK)
  {
    position[ROOK] |= (1ULL << 59);
    position[KING] |= (1ULL << 58);
    position[7] |= (1ULL << 59);
    position[7] |= (1ULL << 58);

    position[ROOK] &= ~(1ULL << 56);
    position[KING] &= ~(1ULL << 60);
    position[7] &= ~(1ULL << 60);
    position[7] &= ~(1ULL << 56);
  } else if (move.promotion != NONE) {
    position[move.promotion] |= (1ULL << move.to);
    position[PAWN] &= ~(1ULL << move.from);

    position[6 + move.color] |= (1ULL << move.to);
    position[6 + move.color] &= ~(1ULL << move.from);
  } else {
    if (position[PAWN] & position[6 + move.color] & (1ULL << move.from))
    {
      position[PAWN] |= (1ULL << move.to);
      position[PAWN] &= ~(1ULL << move.from);
    } else if (position[KNIGHT] & position[6 + move.color] & (1ULL << move.from))
    {
      position[KNIGHT] |= (1ULL << move.to);
      position[KNIGHT] &= ~(1ULL << move.from);
    } else if (position[BISHOP] & position[6 + move.color] & (1ULL << move.from))
    {
      position[BISHOP] |= (1ULL << move.to);
      position[BISHOP] &= ~(1ULL << move.from);
    } else if (position[ROOK] & position[6 + move.color] & (1ULL << move.from))
    {
      position[ROOK] |= (1ULL << move.to);
      position[ROOK] &= ~(1ULL << move.from);
    } else if (position[QUEEN] & position[6 + move.color] & (1ULL << move.from))
    {
      position[QUEEN] |= (1ULL << move.to);
      position[QUEEN] &= ~(1ULL << move.from);
    } else if (position[KING] & position[6 + move.color] & (1ULL << move.from))
    {
      position[KING] |= (1ULL << move.to);
      position[KING] &= ~(1ULL << move.from);
    }

    position[6 + move.color] |= (1ULL << move.to);
    position[6 + move.color] &= ~(1ULL << move.from);
  }
  
  movesList.push_back(move);
  turn = (Color)(1 - turn);
  white_king_castle = move.wkc;
  white_queen_castle = move.wqc;
  black_king_castle = move.bkc;
  black_queen_castle = move.bqc;
  en_passant_square = move.en_passant_target;
}

void Board::print_data()
{
  printf("enpassant_target = %s\nwhite_king_castle = %s\n", POS_STR[en_passant_square].c_str(), white_king_castle ? "true" : "false");
}

void Board::unmakeMove()
{
  if (movesList.empty())
  {
    throw std::runtime_error("Cannot unmake move: No moves have been made.");
  }
  
  Move& lastMove = movesList.back();
  Move& lastToLastMove = (movesList.size() > 1) ? movesList[movesList.size() - 2] : orgGameState;

  turn = lastMove.color; // Notice how turn is the only variable is that relies lastMove 
  white_king_castle = lastToLastMove.wkc;
  white_queen_castle = lastToLastMove.wqc;
  black_king_castle = lastToLastMove.bkc;
  black_queen_castle = lastToLastMove.bqc;
  en_passant_square = lastToLastMove.en_passant_target;
  
  if (lastMove.kc && lastMove.color == WHITE)
  {
    position[ROOK] ^= (1ULL << 5);
    position[KING] ^= (1ULL << 6);
    position[6] ^= (1ULL << 5);
    position[6] ^= (1ULL << 6);

    position[ROOK] |= (1ULL << 7);
    position[KING] |= (1ULL << 4);
    position[6] |= (1ULL << 4);
    position[6] |= (1ULL << 7);

  } else if (lastMove.qc && lastMove.color == WHITE)
  {
    position[ROOK] ^= (1ULL << 3);
    position[KING] ^= (1ULL << 2);
    position[6] ^= (1ULL << 3);
    position[6] ^= (1ULL << 2);

    position[ROOK] |= (1ULL << 0);
    position[KING] |= (1ULL << 4);
    position[6] |= (1ULL << 4);
    position[6] |= (1ULL << 0);
  } else if (lastMove.kc && lastMove.color == BLACK)
  {
    position[ROOK] ^= (1ULL << 61);
    position[KING] ^= (1ULL << 62);
    position[7] ^= (1ULL << 61);
    position[7] ^= (1ULL << 62);

    position[ROOK] |= (1ULL << 63);
    position[KING] |= (1ULL << 60);
    position[7] |= (1ULL << 60);
    position[7] |= (1ULL << 63);
  } else if (lastMove.qc && lastMove.color == BLACK)
  {
    position[ROOK] ^= (1ULL << 59);
    position[KING] ^= (1ULL << 58);
    position[7] ^= (1ULL << 59);
    position[7] ^= (1ULL << 58);

    position[ROOK] |= (1ULL << 56);
    position[KING] |= (1ULL << 60);
    position[7] |= (1ULL << 60);
    position[7] |= (1ULL << 56);
  } else if (lastMove.promotion != NONE) {
    position[lastMove.promotion] ^= (1ULL << lastMove.to);
    position[PAWN] |= (1ULL << lastMove.from);

    position[6 + lastMove.color] ^= (1ULL << lastMove.to);
    position[6 + lastMove.color] |= (1ULL << lastMove.from);
  } else {
    if (position[PAWN] & position[6 + lastMove.color] & (1ULL << lastMove.to))
    {
      position[PAWN] ^= (1ULL << lastMove.to);
      position[PAWN] |= (1ULL << lastMove.from);
    } else if (position[KNIGHT] & position[6 + lastMove.color] & (1ULL << lastMove.to))
    {
      position[KNIGHT] ^= (1ULL << lastMove.to);
      position[KNIGHT] |= (1ULL << lastMove.from);
    } else if (position[BISHOP] & position[6 + lastMove.color] & (1ULL << lastMove.to))
    {
      position[BISHOP] ^= (1ULL << lastMove.to);
      position[BISHOP] |= (1ULL << lastMove.from);
    } else if (position[ROOK] & position[6 + lastMove.color] & (1ULL << lastMove.to))
    {
      position[ROOK] ^= (1ULL << lastMove.to);
      position[ROOK] |= (1ULL << lastMove.from);
    } else if (position[QUEEN] & position[6 + lastMove.color] & (1ULL << lastMove.to))
    {
      position[QUEEN] ^= (1ULL << lastMove.to);
      position[QUEEN] |= (1ULL << lastMove.from);
    } else if (position[KING] & position[6 + lastMove.color] & (1ULL << lastMove.to))
    {
      position[KING] ^= (1ULL << lastMove.to);
      position[KING] |= (1ULL << lastMove.from);
    }

    position[6 + lastMove.color] ^= (1ULL << lastMove.to);
    position[6 + lastMove.color] |= (1ULL << lastMove.from);

  }


  // Deal with captures
  if (lastMove.capture != NONE && lastMove.en_passant_taken == SQ_NONE)
  {
    position[lastMove.capture] |= 1ULL << lastMove.to;
    position[7 - lastMove.color] |= (1ULL << lastMove.to);
  } 
  else if (lastMove.capture != NONE && lastMove.en_passant_taken < SQ_NONE)
  {
    if (lastMove.color == WHITE)
    {
      position[PAWN] |= (1ULL << (lastMove.to - 8));
      position[7] |= (1ULL << (lastMove.to - 8));
    } else {
      position[PAWN] |= (1ULL << (lastMove.to + 8));
      position[6] |= (1ULL << (lastMove.to + 8));
    }
  }

  movesList.pop_back();
  
}

bool Board::in_check()
{
  Bitboard attacks = findUnsafeKingSquares(turn);
  // attacks & (active king)
  return attacks & (position[KING] & position[6 + turn]);
}

KingAttackers Board::findKingAttackers()
{
  KingAttackers attackers;

  if (!in_check()) return attackers;

  Pos kingPos = (Pos)calcLSB(position[6 + turn] & position[5]);
  Bitboard theOpps = position[7 - turn];

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

PinnersPinned Board::findPinnedPieces()
{
  PinnersPinned pinnedPieces;

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
    if (nextBishopAttacks & bishopRays)
    {
      pinnedPieces.push_back({1ULL << nextBishop, nextBishopAttacks & bishopRays, mask_between(nextBishop, kingPos) | 1ULL << nextBishop});
    }

    bishopOpps -= 1ULL << nextBishop;
  }

  // Rooks
  while (rookOpps)
  {
    Pos nextRook = (Pos)calcLSB(rookOpps);
    Bitboard nextRookAttacks = rookAttacks(nextRook, position[6] | position[7]) & position[6 + turn];
    if (nextRookAttacks & rookRays)
    {
      pinnedPieces.push_back({1ULL << nextRook, nextRookAttacks & rookRays, mask_between(nextRook, kingPos) | 1ULL << nextRook});
    }

    rookOpps -= 1ULL << nextRook;
  }

  // Queen
  while (queenOpps)
  {
    Pos nextQueen = (Pos)calcLSB(queenOpps);
    Bitboard nextRookComponent = rookAttacks(nextQueen, position[6] | position[7]) & position[6 + turn];
    Bitboard nextBishopComponent = bishopAttacks(nextQueen, position[6] | position[7]) & position[6 + turn];
    
    if ((nextRookComponent & rookRays & posRookPinner) && (nextQueen/8 == kingPos/8 || abs(nextQueen - kingPos) % 8 == 0))
    {
      pinnedPieces.push_back({1ULL << nextQueen, nextRookComponent & rookRays, mask_between(nextQueen, kingPos) | 1ULL << nextQueen});
    } else if (nextBishopComponent & bishopRays & posBishopPinner && (abs(nextQueen - kingPos) % 7 == 0 | abs(nextQueen - kingPos) % 9 == 0))
    {
      pinnedPieces.push_back({1ULL << nextQueen, nextBishopComponent & bishopRays, mask_between(nextQueen, kingPos) | 1ULL << nextQueen});
    }

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


  auto pinnedPiecesAndPinners = findPinnedPieces();
  Bitboard pinnedPiecesMask = 0ULL;
  for (int s = 0; s < pinnedPiecesAndPinners.size(); ++s)
  {
    pinnedPiecesMask |= pinnedPiecesAndPinners[s][1];
  }

  // Useful for detecting kings walking into checks
  Bitboard kingDanger = findUnsafeKingSquares(turn);
  Bitboard en_passant_mask = (en_passant_square != SQ_NONE) ? 1ULL << en_passant_square : 0ULL;

  KingAttackers attackers = findKingAttackers();

  // std::cout << attackers.size() << std::endl;
  
  for (int i = 0; i < 64; ++i)
  {
    Piece pieceMoving;
    Bitboard allPieceMoves = 0ULL;

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

    if (pinnedPiecesMask & (1ULL << i))
    {
      for (int s = 0; s < pinnedPiecesAndPinners.size(); ++s)
      {
        if (pinnedPiecesAndPinners[s][2] & (1ULL << i))
        {
          allPieceMoves &= pinnedPiecesAndPinners[s][2];

          break;
        }
      }    
    }


    if (attackers.size() == 1 && pieceMoving != KING)
    {
      // Capture piece
      Bitboard possibleCheckDefenseMask = attackers[0].second;

      if (pieceMoving == PAWN)
      {
        possibleCheckDefenseMask &= pawnAttacks((Pos)i, turn);
        if (en_passant_square < SQ_NONE && attackers[0].first == PAWN)
        {
          if (turn == WHITE && (Pos)calcLSB(attackers[0].second) + 8 == en_passant_square)
          {
            possibleCheckDefenseMask |= en_passant_mask;
          } else if (turn == BLACK && (Pos)calcLSB(attackers[0].second) - 8 == en_passant_square)
          {
            possibleCheckDefenseMask |= en_passant_mask;
          }
        }    
      }

      if (attackers[0].first != PAWN && attackers[0].first != KNIGHT)
      {
        try {
          possibleCheckDefenseMask |= mask_between(calcLSB(position[5] & position[6 + turn]),
                                                   calcLSB(attackers[0].second));
        } catch (const std::invalid_argument& e) {
          throw std::invalid_argument("Invalid squares");
        }
      }

      allPieceMoves &= possibleCheckDefenseMask;
    }

    while (allPieceMoves)
    {
      bool can_wkc = white_king_castle;
      bool can_wqc = white_queen_castle;
      bool can_bkc = black_king_castle;
      bool can_bqc = black_queen_castle;

      Pos newEnPassantSquare = SQ_NONE;
      int moveSquare = calcLSB(allPieceMoves);
      Pos en_passant_taken = ((pieceMoving == PAWN) && ((1ULL << moveSquare) & (en_passant_mask))) ? en_passant_square : SQ_NONE; 

      Piece captured_piece;
      
      if (turn == WHITE && (pieceMoving == KING))
      {
        can_wkc = false;
        can_wqc = false;
      } else if (turn == BLACK && (pieceMoving == KING)) {
        can_bkc = false;
        can_bqc = false;
      }
      
      if (turn == WHITE && pieceMoving == ROOK)
      {
        if (i == a1)
        {
          can_wqc = false;
        } else if (i == h1)
        {
          can_wkc = false;
        }
      } else if (turn == BLACK && pieceMoving == ROOK) {
        if (i == a8)
        {
          can_bqc = false;
        } else if (i == h8)
        {
          can_bkc = false;
        }
      }

      if (pieceMoving == PAWN && abs(moveSquare - i) == 16)
      {
        newEnPassantSquare = (Pos)((i + moveSquare) / 2);
      }


      if ((pieceMoving != PAWN && (1ULL << moveSquare & position[7 - turn]))
          || (pawnAttacks((Pos)i, turn) & (1ULL << moveSquare) & position[7 - turn])) {
        for (int i = PAWN; i < KING; ++i)
        {
          if (position[i] & position[7 - turn] & (1ULL << moveSquare))
          {
            captured_piece = (Piece)i;
            if (captured_piece == ROOK)
            {
              if (turn == WHITE)
              {
                  if (moveSquare == h8)
                  {
                    can_bkc = false;
                  } else if (moveSquare == a8) {
                    can_bqc = false;
                  }
              } else if (turn == BLACK) {
                  if (moveSquare == h1)
                  {
                    can_wkc = false;
                  } else if (moveSquare == a1) {
                    can_wqc = false;
                  }
              }
            }
            break;
          }
        }
      } else if (en_passant_taken < SQ_NONE) {
        captured_piece = PAWN;
      } else {
        captured_piece = NONE;
      }
       
      if (pieceMoving == PAWN && ((allPieceMoves) & (RANK_1 | RANK_8)))
      {
        legal_moves.push_back(
          Move((Pos)i, (Pos)moveSquare, turn, false, false, newEnPassantSquare, KNIGHT, can_wkc, can_wqc, can_bkc, can_bqc, captured_piece)
        );

        legal_moves.push_back(
          Move((Pos)i, (Pos)moveSquare, turn, false, false, newEnPassantSquare, BISHOP, can_wkc, can_wqc, can_bkc, can_bqc, captured_piece)
        );

        legal_moves.push_back(
          Move((Pos)i, (Pos)moveSquare, turn, false, false, newEnPassantSquare, ROOK, can_wkc, can_wqc, can_bkc, can_bqc, captured_piece)
        );

        legal_moves.push_back(
          Move((Pos)i, (Pos)moveSquare, turn, false, false, newEnPassantSquare, QUEEN, can_wkc, can_wqc, can_bkc, can_bqc, captured_piece)
        );
      } else {
        legal_moves.push_back(
          Move((Pos)i, (Pos)moveSquare, turn, false, false, newEnPassantSquare, NONE,
               can_wkc, can_wqc, can_bkc, can_bqc, captured_piece, en_passant_taken)
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
        legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, true, false, SQ_NONE, NONE, false, false, black_king_castle, black_queen_castle));
      }


      if (white_queen_castle &&
        !(kingDanger & (1ULL << 2)) && !(allColorPieces & (1ULL << 2)) && !(allOppPieces & (1ULL << 2)) &&
        !(kingDanger & (1ULL << 3)) && !(allColorPieces & (1ULL << 3)) && !(allOppPieces & (1ULL << 3)) &&
        !(allColorPieces & (1ULL << 1)) && !(allOppPieces & (1ULL << 1)))
      {
        legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, false, true, SQ_NONE, NONE, false, false, black_king_castle, black_queen_castle));
      }
    }
    else
    {

      if (black_king_castle && !(kingDanger & (1ULL << 61)) && !(allColorPieces & (1ULL << 61)) && !(allOppPieces & (1ULL << 61)) &&
        !(kingDanger & (1ULL << 62)) && !(allColorPieces & (1ULL << 62)) && !(allOppPieces & (1ULL << 62)))
      {
        legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, true, false, SQ_NONE, NONE, white_king_castle, white_queen_castle, false, false));
      }
      if (black_queen_castle &&
        !(kingDanger & (1ULL << 58)) && !(allColorPieces & (1ULL << 58)) && !(allOppPieces & (1ULL << 58)) &&
        !(kingDanger & (1ULL << 59)) && !(allColorPieces & (1ULL << 59)) && !(allOppPieces & (1ULL << 59)) &&
        !(allColorPieces & (1ULL << 57)) && !(allOppPieces & (1ULL << 57)))
      {
        legal_moves.push_back(Move(SQ_NONE, SQ_NONE, turn, false, true, SQ_NONE, NONE, white_king_castle, white_queen_castle, false, false));
      }
    }
  }
  
  //if (movesList.size() > 0 && (movesList.back().to == e4))
  //{
  //  printf("%s\n", movesList.back().wkc ? "true" : "false");
  //  print_data();
  //  //Board::print_moves(legal_moves);
  //}
   
  return legal_moves;

}
