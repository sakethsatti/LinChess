#pragma once

#include <chess/constants.hpp>
#include <chess/types.hpp>
#include <stdexcept>
#include <chess/bitboard.hpp>
#include <iostream>

#define ASSERT_WITH_POSITION(condition, board)                                  \
    do {                                                                        \
        if (!(condition)) {                                                     \
            std::cerr << "Assertion failed: " << #condition << "\n";            \
            board.print_position();                                             \
            board.print_moves(board.movesList);                                 \
            std::cout << board.currState->last_taken << "\n";                   \
            std::cerr << "File: " << __FILE__ << "\nLine: " << __LINE__ << '\n';\
            std::exit(EXIT_FAILURE);                                            \
        }                                                                       \
    } while (false)   



struct GameInfo {
  Color turn;
  int halfmove_clock;
  int move_number; 
  GameInfo* prev;
  CastlingInfo ci;
  
  // En Passant target
  Pos ep;
  
  Piece last_taken;
  
  GameInfo(Color turn = WHITE, int halfmove_clock = 0, int move_number = 1,
           CastlingInfo ci = CastlingInfo(true, true, true, true),
           Pos ep = SQ_NONE, Piece last_taken = Piece::NONE, GameInfo* prev = nullptr): 
    turn(turn), halfmove_clock(halfmove_clock), move_number(move_number),
    ci(ci), ep(ep), last_taken(last_taken), prev(prev) {} 

  bool operator==(const GameInfo& otherGI) const;

  friend std::ostream& operator<<(std::ostream& os, const GameInfo& gi);
};

inline bool GameInfo::operator==(const GameInfo& otherGI) const {
  if (!(turn == otherGI.turn && halfmove_clock == otherGI.halfmove_clock &&
    move_number == otherGI.move_number && prev == otherGI.prev &&
    ep == otherGI.ep && last_taken == otherGI.last_taken)) return false;
  if (!(ci == otherGI.ci)) return false;
  return true;
}

inline std::ostream& operator<<(std::ostream& os, const GameInfo& gi) {
  os << "Turn: " << (gi.turn == WHITE ? "White" : "Black")
     << ", Halfmove clock: " << gi.halfmove_clock
     << ", Move number: " << gi.move_number
     << ", Castling info: " << gi.ci
     << ", En passant target: " << POS_STR[gi.ep]
     << ", Last taken piece: " << (int)gi.last_taken;
  
  if (gi.prev != nullptr) {
    os << ", Previous state exists" ;
  } else {
    os << ", No previous state";
  }

  return os;
}


/*
 * Board Class. This class contains everything
 * about a position including a pointer to a 
 * state.
*/


class Board {
private:

  /*
   * Represents the entire position using an array of bitboards
   * 0 - pawns 
   * 1 - knights
   * 2 - bishops
   * 3 - rooks
   * 4 - queens
   * 5 - kings
   * 6 - white pieces
   * 7 - black pieces
  */
  array<Bitboard, 8> position;
  GameInfo* currState;

  array<Piece, 64> pieceBoard;
  void setPieceBoard();

  void removePiece(const Pos& sq, const Color& color);
  void movePiece(const Pos& from, const Pos& to, const Color& color); 
  void castle(const Pos& to);
  void uncastle(const Pos& to);
  void addPiece(const Pos& sq, const Color& color, const Piece& newPiece);

  Bitboard findUnsafeKingSquares(Color color);
  Bitboard attacksBySliders(Bitboard bishops, Bitboard rooks, Bitboard queens, Bitboard allPieces);
  KingAttackers findKingAttackers();
  bool in_check();
  PinnersPinned findPinnedPieces();

  vector<Move> movesList; 
  
public:    
  Board();
  Board(string FEN);
  ~Board() {
    delete currState;
    currState = nullptr;
  }

  Board(const Board&) = delete;
  Board& operator=(const Board&) = delete; 

  bool operator==(const Board &b) const; 

  void print_position();

  void makeMove(const Move& move);
  void unmakeMove();

  MovesList genLegalMoves();

  Piece pieceOnSquare(const Pos& sq);
  void print_data();
  Color getTurn(); 
  static void print_moves(const MovesList& legalMove);
  
  friend void positionOk(Board& b);
};

// Inline functions

inline void positionOk(Board& b) {
  ASSERT_WITH_POSITION(count_bits(b.position[KING]) == 2, b);
  ASSERT_WITH_POSITION(count_bits(b.position[6 + WHITE]) <= 16, b);
  ASSERT_WITH_POSITION(count_bits(b.position[6 + BLACK]) <= 16, b); 
  ASSERT_WITH_POSITION(!(b.position[PAWN] & (RANK_1 | RANK_8)), b);
}

inline void Board::print_moves(const MovesList& legalMove)
{
  for (Move move : legalMove)
  {
    if (move.moveType() == CASTLING && (move.to() == g1 && move.to() == g8))
    {
      std::cout << "O-O" << std::endl;
    } else if (move.moveType() == CASTLING && (move.to() == b1 && move.to() == b8)) {
      std::cout << "O-O-O" << std::endl;
    } else {
      std::cout << POS_STR[move.from()] << POS_STR[move.to()] << std::endl;
    }
  }
}

inline bool Board::operator==(const Board& b) const {
  if (position != b.position) {
    return false;
  }

  if (!( *currState == *(b.currState) ))
  {
    return false;
  }
  return true;
}

inline Piece Board::pieceOnSquare(const Pos& sq) {
  return pieceBoard[sq]; 
}

inline void Board::addPiece(const Pos& sq, const Color& color, const Piece& newPiece) {
  position[newPiece] |= 1ULL << sq;
  position[6 + color] |= 1ULL << sq;

  pieceBoard[sq] = newPiece;
}

inline void Board::removePiece(const Pos& sq, const Color& color) {
  Piece type = pieceOnSquare(sq); 
  if (type == NONE) {
    std::cout << "removePiece \n";
    std::cout << POS_STR[sq] << "\n";
    print_moves(movesList);
    print_position();
    throw std::invalid_argument("No piece on square");
  } 

  position[type] ^= 1ULL << sq;
  position[6 + color] ^= 1ULL << sq;

  pieceBoard[sq] = NONE;
}

inline void Board::movePiece(const Pos& from, const Pos& to, const Color& color) {
  Piece type = pieceOnSquare(from);

  if (type == NONE){
    std::cout << "movePiece \n";
    print_moves(movesList);
    print_position();
    throw std::invalid_argument("No piece on square");
  }
  
  removePiece(from, color);
  addPiece(to, color, type);
}

inline void Board::castle(const Pos& to){
  if (to == g1) {

    movePiece(e1, g1, WHITE);
    movePiece(h1, f1, WHITE);

  } else if (to == c1) {

    movePiece(e1, c1, WHITE);
    movePiece(a1, d1, WHITE);

  } else if (to == g8) {

    movePiece(e8, g8, BLACK);
    movePiece(h8, f8, BLACK);

  } else if (to == c8) {

    movePiece(e8, c8, BLACK);
    movePiece(a8, d8, BLACK);

  }
} 

inline void Board::uncastle(const Pos& to) {
  if (to == g1) {

    movePiece(g1, e1, WHITE);
    movePiece(f1, h1, WHITE);

  } else if (to == c1) {

    movePiece(c1, e1, WHITE);
    movePiece(d1, a1, WHITE);

  } else if (to == g8) {

    movePiece(g8, e8, BLACK);
    movePiece(f8, h8, BLACK);

  } else if (to == c8) {

    movePiece(c8, e8, BLACK);
    movePiece(d8, a8, BLACK);

  }
}

inline Color Board::getTurn() {return currState->turn;}

long perft(Board &b, const int& depth, const int& oDepth);
void perftRunner(Board &b, const int& depth, const int& oDepth); 

