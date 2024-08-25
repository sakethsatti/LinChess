#include <chess/board.hpp>
#include <chrono>
#include <iostream>

int perft(Board &b, const int& depth, const int& oDepth)
{
  int count = 0;
  const MovesList& moves = b.genLegalMoves();
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
    // auto lol = copier(b);

    b.makeMove(move);
    // auto moveMade = copier(b);
    
    int temp = perft(b, depth - 1, oDepth);
    if (depth == oDepth) {
      Board::print_moves(MovesList {move});
      std::cout << temp << std::endl;
      std::cout << std::endl;
    }
    count += temp;

    b.unmakeMove();
    /*
    if (b != lol)
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
    */
  }
  return count;

}

void perftRunner(Board &b, const int& depth, const int& oDepth)
{
  auto start = std::chrono::high_resolution_clock::now();
 
  int count = perft(b, depth, oDepth);  
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Nodes: " << count << std::endl; 
  std::chrono::duration<double, std::milli> duration = end - start;
  double seconds = duration.count() / 1000.0;
  std::cout << "Time elapsed: " <<  seconds << " seconds" << std::endl;
  std::cout << "Nodes/sec: " << count/seconds << std::endl;
}
