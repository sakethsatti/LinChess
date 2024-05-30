#include <iostream>
#include <bitset>
#include "ChessGame/magics.hpp"

int main() {    
    auto magics = findAllMagics(ROOK);

    for (U64 magic : magics) {
        std::cout << "0x" << std::hex << magic << ", ";
    }


    return 0;
}