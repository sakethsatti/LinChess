#include "board.hpp"
#include <stdexcept>

int perftRunner(Board &b, const int& depth, const int& oDepth)
{
  
  int count = 0;
  const LegalMoves& moves = b.genLegalMoves();
  if (depth == 1)
  {
    if (depth == oDepth)
    {
      Board::print_moves(moves);
    }
    return moves.size();
  }
  
  
  for (Move move : moves)
  {
    auto lol = copier(b);

    b.moveMaker(move);
    auto moveMade = copier(b);
    
    int temp = perftRunner(b, depth - 1, oDepth);
    if (depth == oDepth) {
      Board::print_moves(LegalMoves {move});
      std::cout << temp << std::endl;
      std::cout << std::endl;
    }
    count += temp;

    b.unmakeMove();

    if (!b.equals(lol))
    {
      lol.print_position(); 
      b.print_position();
      moveMade.print_position();
      // std::cout << moveMade.movesList.back().capture << std::endl;
      Board::print_moves(moveMade.movesList);
      // std::cout << b.getTurn() << std::endl;
      //lol.print_data();
      //moveMade.print_data();
      //b.print_data();

      throw std::invalid_argument("Ok Bud");
    }
    
  }

  return count;

}

Board copier(Board b)
{
  return b;
}
