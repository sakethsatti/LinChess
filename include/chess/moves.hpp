#include "constants.hpp"

struct GameState {
  
};

/*
  * Bits represent different things
  * 0-5: from
  * 6-11: to
  * 12-13: promotion type
  * 14-15: 00 promotion, 01 en passant, 10 king caslte, 11 queen castle 
*/
struct Move { 
  uint16_t moveInfo;
   

  Pos from;
  Pos to;

  Pos en_passant_target;
  Pos en_passant_taken;  

  Color color;

  // Move was a castle
  bool kc;
  bool qc;

  // Ability to castle
  bool wkc;
  bool wqc;
  bool bkc;
  bool bqc;

  Piece promotion;
  Piece capture;
  bool first_move;

  Move(Pos from, Pos to, Color color, bool king_castle, bool queen_castle, Pos en_passant_target, Piece promotion,
       bool wkc = true, bool wqc = true, bool bkc = true, bool bqc = true, Piece capture = NONE, Pos en_passant_taken = SQ_NONE) :
    from(from), to(to), kc(king_castle), qc(queen_castle), color(color), en_passant_target(en_passant_target),
    promotion(promotion), wkc(wkc), wqc(wqc), bkc(bkc), bqc(bqc), capture(capture), en_passant_taken(en_passant_taken) {}

};

