#include "gameState.h"
#include "piece.h"
#include "player.h"
#include <iostream>

int main() {
    std::cout << "Starting chess engine...\n";

    // Create game state
    GameState game;

    // Create players
    Player* white = new Player(&game, true);
    Player* black= new Player(&game, false);

    // Initialize board
    game.init();
    game.printBoard();

    // Example: make a move (if your API supports it)
    // white.movePiece({'e',2}, {'e',4});

    std::cout << "Program finished.\n";
    return 0;
}