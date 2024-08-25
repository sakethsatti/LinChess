#include <chess/board.hpp>
#include <chess/bitboard.hpp>
#include <cassert>
#include <cstring>
#include <sstream>
#include <stdexcept>

Board::Board()
{
  currState = new GameInfo();
  // Initialize position
  position[0] = RANK_2 | RANK_7;
  position[1] = (1ULL << 1) | (1ULL << 6) | (1ULL << 57) | (1ULL << 62);
  position[2] = (1ULL << 2) | (1ULL << 5) | (1ULL << 58) | (1ULL << 61);
  position[3] = (1ULL << 0) | (1ULL << 7) | (1ULL << 56) | (1ULL << 63);
  position[4] = (1ULL << 3) | (1ULL << 59);
  position[5] = (1ULL << 4) | (1ULL << 60);
  position[6] = RANK_2 | RANK_1;
  position[7] = RANK_7 | RANK_8;

  setPieceBoard();
}

Board::Board(std::string FEN)
{
  currState = new GameInfo();

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
    currState->turn = WHITE;
  } else {
    currState->turn = BLACK;
  }

  bool white_king_castle = false;
  bool black_king_castle = false;
  bool white_queen_castle = false;
  bool black_queen_castle = false;

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
  currState->ci = CastlingInfo(white_king_castle, white_queen_castle, black_king_castle, black_queen_castle);

  if (FEN_parts[3] != "-") {
    currState->ep = (Pos)(FEN_parts[3][0] - 'a' + 8 * (FEN_parts[3][1] - '1'));
  } else {
    currState->ep = SQ_NONE;
  }

  currState->halfmove_clock = (FEN_parts[4] != "") ? std::stoi(FEN_parts[4]) : 0;
  currState->move_number = (FEN_parts[5] != "") ? std::stoi(FEN_parts[5]) : 0;

  setPieceBoard();
}

void Board::setPieceBoard() {
  pieceBoard = {NONE};

  for (int i = 0; i < 64; ++i) {

    if ((1ULL << i) & position[PAWN]) {

      pieceBoard[i] = PAWN;

    } else if ((1ULL << i) & position[KNIGHT]) {

      pieceBoard[i] = KNIGHT;

    } else if ((1ULL << i) & position[BISHOP]) {

      pieceBoard[i] = BISHOP;

    } else if ((1ULL << i) & position[ROOK]) {

      pieceBoard[i] = ROOK;

    } else if ((1ULL << i) & position[QUEEN]) {

      pieceBoard[i] = QUEEN;

    } else if ((1ULL << i) & position[KING]) {

      pieceBoard[i] = KING;

    } else {

      pieceBoard[i] = Piece::NONE;

    }

  }
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
  Bitboard oppPiecesCopy = oppPieces;

  // Notice the use of 6 + turn get the opposite color's pieces 
  while (oppPiecesCopy)
  {
    int lsb = calcLSB(oppPiecesCopy);
    Piece piece = pieceOnSquare((Pos)lsb);
    
    switch (piece) {
      case PAWN:
        attacks |= pawnAttacks((Pos)lsb, Color(1 - turn));
        break;
      case KNIGHT:
        attacks |= knightAttacks((Pos)lsb);
        break;
      case BISHOP:
        attacks |= bishopAttacks((Pos)lsb, friendPieces | oppPieces);
        break;
      case ROOK:        
        attacks |= rookAttacks((Pos)lsb, friendPieces | oppPieces);
        break;
      case QUEEN:    
        attacks |= queenAttacks((Pos)lsb, friendPieces | oppPieces);
        break;
      case KING:
        attacks |= kingAttacks((Pos)lsb);
        break;
      default:
        break;
    }
    
    oppPiecesCopy &= oppPiecesCopy - 1;
  }
  
  return attacks;

}

void Board::makeMove(const Move& move)
{

  // Create a new GameInfo object
  GameInfo* newInfo = new GameInfo();

  // Copy current state
  std::memcpy(newInfo, currState, sizeof(GameInfo));

  // Update the new state
  newInfo->move_number = currState->move_number + (currState->turn == BLACK ? 1 : 0);
  newInfo->halfmove_clock = currState->halfmove_clock + 1;
  newInfo->prev = currState;
  newInfo->last_taken = Piece::NONE; // Reset taken piece

  Color moveSide = currState->turn;
  Color opps = ~moveSide;
  newInfo->turn = opps;

  Piece pieceMoving = pieceOnSquare(move.from());

  // Update castling rights
  if (pieceMoving == KING) {

    if (moveSide == WHITE) newInfo->ci.setWC(false, false);
    else newInfo->ci.setBC(false, false);

  } else if (pieceMoving == ROOK) {

    if (move.from() == h1) newInfo->ci.setWKC(false);
    else if (move.from() == a1) newInfo->ci.setWQC(false);
    else if (move.from() == h8) newInfo->ci.setBKC(false);
    else if (move.from() == a8) newInfo->ci.setBQC(false);

  }

  // Reset halfmove clock for pawn moves or captures
  if (pieceMoving == PAWN || pieceOnSquare(move.to()) != Piece::NONE) {

    newInfo->halfmove_clock = 0;

  }

  // Handle captures
  if (pieceOnSquare(move.to()) != Piece::NONE) {
    if (pieceOnSquare(move.to()) == ROOK) {
      if (move.to() == h1) newInfo->ci.setWKC(false);
      else if (move.to() == a1) newInfo->ci.setWQC(false);
      else if (move.to() == h8) newInfo->ci.setBKC(false);
      else if (move.to() == a8) newInfo->ci.setBQC(false);
    }

    newInfo->last_taken = pieceOnSquare(move.to());
    removePiece(move.to(), opps);

  }

  // Handle special moves
  switch (move.moveType()) {
    case EN_PASSANT:
      newInfo->last_taken = PAWN;

      Pos captureSQ;
      if (getTurn() == WHITE) captureSQ = Pos(currState->ep - 8);
      else captureSQ = Pos(currState->ep + 8);
      removePiece(captureSQ, opps);

      movePiece(move.from(), move.to(), moveSide);
      break;
    case CASTLING:
      castle(move.to());
      break;
    case PROMOTION:
      assert(move.promotion_type() != KING);
      removePiece(move.from(), moveSide);
      addPiece(move.to(), moveSide, move.promotion_type());
      break;
    default:
      movePiece(move.from(), move.to(), moveSide);
  }

  // Update en passant target
  newInfo->ep = (pieceMoving == PAWN && abs(move.to() - move.from()) == 16) 
    ? Pos((move.from() + move.to()) / 2) 
    : SQ_NONE;

  //std::cout << (*currState) << std::endl;
  //std::cout << (*newInfo) << std::endl;

  // Update the current state
  currState = newInfo;
  movesList.push_back(move);
}

void Board::print_data()
{
  printf("enpassant_target = %s\nwhite_king_castle = %s\n", POS_STR[currState->ep].c_str(), currState->ci.wkc() ? "true" : "false");
}

void Board::unmakeMove()
{

  positionOk(*this);
  if (movesList.empty())
  {
    throw std::runtime_error("Cannot unmake move: No moves have been made.");
  }

  Move& lastMove = movesList.back();
  Color sideInactive = currState->turn;
  Color sideActive = ~sideInactive;

  // Undo the move based on its type
  switch (lastMove.moveType()) {
    case EN_PASSANT:
      Pos captureSQ;

      if (sideActive == WHITE) captureSQ = Pos(currState->prev->ep - 8);
      else captureSQ = Pos(currState->prev->ep + 8);

      addPiece(captureSQ, sideInactive, PAWN);
      movePiece(lastMove.to(), lastMove.from(), sideActive);

      break;
    case CASTLING:
      uncastle(lastMove.to()); 
      break;
    case PROMOTION:
      removePiece(lastMove.to(), sideActive);
      addPiece(lastMove.from(), sideActive, PAWN);
      if (currState->last_taken != NONE) addPiece(lastMove.to(), sideInactive, currState->last_taken);
      break;
    default:
      movePiece(lastMove.to(), lastMove.from(), sideActive);
      if (currState->last_taken != NONE) addPiece(lastMove.to(), sideInactive, currState->last_taken);
  }

  // Store the pointer to the previous state
  GameInfo* prevState = currState->prev;
  // Delete the current state
  delete currState;

  // Set the current state to the previous state
  currState = prevState;

  movesList.pop_back();

}


bool Board::in_check()
{
  Bitboard attacks = findUnsafeKingSquares(getTurn());
  // attacks & (active king)
  return attacks & (position[KING] & position[6 + getTurn()]);
}

KingAttackers Board::findKingAttackers()
{
  KingAttackers attackers;

  if (!in_check()) return attackers;

  Pos kingPos = (Pos)calcLSB(position[6 + getTurn()] & position[5]);
  Bitboard theOpps = position[7 - getTurn()];

  // Find all pieces that seem to be attacking the king
  // King cannot directly attack the king so we don't need to check for that
  Bitboard pawnAttackers = pawnAttacks(kingPos, getTurn()) & position[0] & theOpps & position[0];
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
    queenAttackers &= (queenAttackers - 1);
  }

  return attackers;
}

PinnersPinned Board::findPinnedPieces()
{
  PinnersPinned pinnedPieces;

  // Get all the opposition sliders to iterate through
  Bitboard bishopOpps = position[7 - getTurn()] & position[BISHOP];
  Bitboard rookOpps = position[7 - getTurn()] & position[ROOK];
  Bitboard queenOpps = position[7 - getTurn()] & position[QUEEN];

  // Get king position
  Pos kingPos = (Pos)calcLSB(position[KING] & position[6 + getTurn()]);

  // Get rays as if there was a opp in kings position
  Bitboard bishopRays = bishopAttacks(kingPos, position[6] | position[7]) & position[6 + getTurn()];
  Bitboard rookRays = rookAttacks(kingPos, position[6] | position[7]) & position[6 + getTurn()];

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
    Bitboard nextBishopAttacks = bishopAttacks(nextBishop, position[6] | position[7]) & position[6 + getTurn()];
    if (nextBishopAttacks & bishopRays)
    {
      pinnedPieces.push_back({1ULL << nextBishop, nextBishopAttacks & bishopRays, mask_between(nextBishop, kingPos) | 1ULL << nextBishop});
    }

    bishopOpps &= bishopOpps - 1;
  }

  // Rooks
  while (rookOpps)
  {
    Pos nextRook = (Pos)calcLSB(rookOpps);
    Bitboard nextRookAttacks = rookAttacks(nextRook, position[6] | position[7]) & position[6 + getTurn()];
    if (nextRookAttacks & rookRays)
    {
      pinnedPieces.push_back({1ULL << nextRook, nextRookAttacks & rookRays, mask_between(nextRook, kingPos) | 1ULL << nextRook});
    }

    rookOpps &= rookOpps - 1;
  }

  // Queen
  while (queenOpps)
  {
    Pos nextQueen = (Pos)calcLSB(queenOpps);
    Bitboard nextRookComponent = rookAttacks(nextQueen, position[6] | position[7]) & position[6 + getTurn()];
    Bitboard nextBishopComponent = bishopAttacks(nextQueen, position[6] | position[7]) & position[6 + getTurn()];

    if ((nextRookComponent & rookRays & posRookPinner) && (nextQueen/8 == kingPos/8 || abs(nextQueen - kingPos) % 8 == 0))
    {
      pinnedPieces.push_back({1ULL << nextQueen, nextRookComponent & rookRays, mask_between(nextQueen, kingPos) | 1ULL << nextQueen});
    } else if (nextBishopComponent & bishopRays & posBishopPinner && (abs(nextQueen - kingPos) % 7 == 0 | abs(nextQueen - kingPos) % 9 == 0))
    {
      pinnedPieces.push_back({1ULL << nextQueen, nextBishopComponent & bishopRays, mask_between(nextQueen, kingPos) | 1ULL << nextQueen});
    }

    queenOpps &= queenOpps - 1;
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
    rooks &= rooks - 1;
  }

  while (bishops)
  {
    Pos nextBishop = (Pos)calcLSB(bishops);
    attacks |= bishopAttacks(nextBishop, allPieces);
    bishops &= bishops - 1;
  }

  while (queens)
  {
    Pos nextQueen = (Pos)calcLSB(queens);
    attacks |= queenAttacks(nextQueen, allPieces);
    queens &= queens - 1;
  }

  return attacks;

}

MovesList Board::genLegalMoves()
{
  Bitboard allColorPieces;
  Bitboard allOppPieces;

  bool is_in_check = in_check();

  if (getTurn() == WHITE)
  {
    allColorPieces = position[6];
    allOppPieces = position[7];
  }
  else {
    allColorPieces = position[7];
    allOppPieces = position[6];
  }
  MovesList legal_moves;

  Bitboard copyForIteration = allColorPieces;

  // print_bitboard(allColorPieces);
  // print_bitboard(allOppPieces);


  auto pinnedPiecesAndPinners = findPinnedPieces();
  Bitboard pinnedPiecesMask = 0ULL;
  for (int s = 0; s < pinnedPiecesAndPinners.size(); ++s)
  {
    pinnedPiecesMask |= pinnedPiecesAndPinners[s][1];
  }

  // Useful for detecting kings walking into checks
  Bitboard kingDanger = findUnsafeKingSquares(getTurn());
  Pos en_passant_square = currState->ep;
  Bitboard en_passant_mask = (currState->ep != SQ_NONE) ? 1ULL << currState->ep : 0ULL;

  KingAttackers attackers = findKingAttackers();
  
  // std::cout << attackers.size() << std::endl;

  while(copyForIteration)
  {
    int i = calcLSB(copyForIteration);

    Piece pieceMoving = pieceOnSquare(Pos(i));
    Bitboard allPieceMoves = 0ULL;

    // If the piece isn't a king and there are multiple attackers, only the king can move
    if (attackers.size() > 1 && pieceMoving != KING)
    { 
      copyForIteration &= copyForIteration - 1;
      continue;
    }

    switch (pieceMoving)
    {
      case KING:
        {
          allPieceMoves = kingAttacks((Pos)i) & ~kingDanger & ~allColorPieces;
          break;
        }
      
      case PAWN:
        {
          allPieceMoves = pawnMoves((Pos)i, allColorPieces | allOppPieces, getTurn());

          Bitboard attackingSquares = pawnAttacks((Pos)i, getTurn());

          allPieceMoves |= attackingSquares & allOppPieces;

          if (attackingSquares & en_passant_mask)
          {
            Bitboard newPosition = (position[6] | position[7] | (1ULL << en_passant_square)) &
              ~(
              (1ULL << i) |
              ((getTurn() == WHITE) ? (1ULL << (en_passant_square - 8)) : (1ULL << (en_passant_square + 8)))
            );

            Bitboard newAttacked = attacksBySliders(
              position[2] & position[7 - getTurn()],
              position[3] & position[7 - getTurn()],
              position[4] & position[7 - getTurn()],
              newPosition
            );

            if (!(newAttacked & (position[5] & position[6 + getTurn()])))
            {
              allPieceMoves |= en_passant_mask; 
            }
          }
          break;
        }

      case KNIGHT:
        {
          allPieceMoves = knightAttacks((Pos)i) & ~allColorPieces;
          break;
        }

      case BISHOP:
        {
          allPieceMoves = bishopAttacks((Pos)i, allColorPieces | allOppPieces) & ~allColorPieces;
          break;
        }

      case ROOK:
        {
          allPieceMoves = rookAttacks((Pos)i, allColorPieces | allOppPieces) & ~allColorPieces;
          break;
        }

      case QUEEN:
        {
          allPieceMoves = queenAttacks((Pos)i, allColorPieces | allOppPieces) & ~allColorPieces;
          break;
        }

      default:
        break;
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
        possibleCheckDefenseMask &= pawnAttacks((Pos)i, getTurn());
        if (en_passant_square < SQ_NONE && attackers[0].first == PAWN)
        {
          if (getTurn() == WHITE && (Pos)calcLSB(attackers[0].second) + 8 == en_passant_square)
          {
            possibleCheckDefenseMask |= en_passant_mask;
          } else if (getTurn() == BLACK && (Pos)calcLSB(attackers[0].second) - 8 == en_passant_square)
          {
            possibleCheckDefenseMask |= en_passant_mask;
          }
        }    
      }

      if (attackers[0].first != PAWN && attackers[0].first != KNIGHT)
      {
        try {
          possibleCheckDefenseMask |= mask_between(calcLSB(position[5] & position[6 + getTurn()]),
                                                   calcLSB(attackers[0].second));
        } catch (const std::invalid_argument& e) {
          throw std::invalid_argument("Invalid squares");
        }
      }

      allPieceMoves &= possibleCheckDefenseMask;
    }
    
    while (allPieceMoves)
    { 
      Pos moveSquare = (Pos)calcLSB(allPieceMoves);
      bool en_passant_taken = ((pieceMoving == PAWN) && ((1ULL << moveSquare) & (en_passant_mask))) ? true : false; 

      Piece captured_piece;

      if (pieceMoving == PAWN && ((allPieceMoves) & (RANK_1 | RANK_8)))
      {
        for (int p = KNIGHT; p < KING; ++p) {
          legal_moves.push_back(
            Move::create<PROMOTION>((Pos)i, moveSquare, (Piece)p)  
          );
        } 
      } else if (en_passant_taken) {
        legal_moves.push_back(
          Move::create<EN_PASSANT>((Pos)i, moveSquare)
        );
      } else {
        legal_moves.push_back(
          Move::create<NORMAL>((Pos)i, moveSquare)
        );
      }

      allPieceMoves &= allPieceMoves - 1;
    }
    copyForIteration &= copyForIteration - 1;
  }

  if(attackers.size() == 0)
  {
    if (getTurn() == WHITE)
    {
      if (currState->ci.wkc() && !(WKC_SQUARES & (allColorPieces | allOppPieces | kingDanger)))
      {
        legal_moves.push_back(Move::create<CASTLING>(e1, g1));
      }


      if (currState->ci.wqc() && !(WQC_SQUARES & (allColorPieces | allOppPieces)) && !((1ULL << c1 | 1ULL << d1) & kingDanger))
      {    
        legal_moves.push_back(Move::create<CASTLING>(e1, c1));
      }
    }
    else
    {

      if (currState->ci.bkc() && !(BKC_SQUARES & (allColorPieces | allOppPieces | kingDanger)))
      {
        legal_moves.push_back(Move::create<CASTLING>(e8, g8));
      }
      if (currState->ci.bqc() && !(BQC_SQUARES & (allColorPieces | allOppPieces)) && !((1ULL << c8 | 1ULL << d8) & kingDanger))
      {
        legal_moves.push_back(Move::create<CASTLING>(e8, c8));
      }
    }
  }

  //if (movesList.size() > 0 && (movesList.back().to() == h8))
  //{
  //  Board::print_moves(legal_moves);
  //}

  return legal_moves;

}
