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

    Player *current = white;

     while (true) {
        game.printBoard();

        Player::TurnResult result = current->turn();

        switch (result) {
            case Player::TurnResult::Checkmate:
                std::cout << "Checkmate! "
                          << (current->color ? "White" : "Black")
                          << " loses.\n";
                return 0;

            case Player::TurnResult::Stalemate:
                std::cout << "Stalemate. Draw.\n";
                return 0;

            case Player::TurnResult::Resign:
                std::cout << (current->color ? "White" : "Black")
                          << " resigns. "
                          << (current->color ? "Black" : "White")
                          << " wins.\n";
                return 0;

            case Player::TurnResult::Exit:
                std::cout << "Game ended by user.\n";
                return 0;
            case Player::TurnResult::Continue:
                break;
            }

        // Switch players
        current = (current == white ? black : white);
    }

    return 0;
}
