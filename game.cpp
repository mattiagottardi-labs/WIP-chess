#include "game.h"
#include <iostream>

#include "pieces/piece.h"
#include "pieces/rook.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "pieces/pawn.h"
#include "pieces/empty.h"
Game game;
void Game::initGame() {
    board[{0,0}] = new Rook({0,0}, 0, true);
    board[{1,0}] = new Knight({1,0}, 0, true);
    board[{2,0}] = new Bishop({2,0}, 0, true);
    board[{3,0}] = new Queen({3,0}, 0, true);
    board[{4,0}] = new King({4,0}, 0, true);
    board[{5,0}] = new Bishop({5,0}, 0, true);
    board[{6,0}] = new Knight({6,0}, 0, true);
    board[{7,0}] = new Rook({7,0}, 0, true);

    for(int i = 0 ; i < 8; i++){
        board[{i,1}] = new Pawn({i,1}, 0, true);
        board[{i,6}] = new Pawn({i,6}, 1, true);
    }

    board[{0,7}] = new Rook({0,7}, 1, true);
    board[{1,7}] = new Knight({1,7}, 1, true);
    board[{2,7}] = new Bishop({2,7}, 1, true);
    board[{3,7}] = new Queen({3,7}, 1, true);
    board[{4,7}] = new King({4,7}, 1, true);
    board[{5,7}] = new Bishop({5,7}, 1, true);
    board[{6,7}] = new Knight({6,7}, 1, true);
    board[{7,7}] = new Rook({7,7}, 1, true);

    for(int i = 0 ; i < 8; i++){
        for(int j = 2 ; j < 6; j++){
            board[{i,j}] = new Empty({i,j}, 2, false);
        }
    }
}

void Game::printBoard() {
    std::cout << "\n  0  1  2  3  4  5  6  7\n";
    for(int j = 7 ; j >= 0; j--){j;
        std::cout << j << " ";
        for(int i = 0 ; i < 8; i++){
            if(board[{i,j}]->isEmpty()){
                std::cout << " . ";
            } else {
                std::cout << board[{i,j}]->getName() << " ";
            }
        }
        std::cout << j;
        std::cout << "\n\n";
    } std::cout << "  0  1  2  3  4  5  6  7\n";
}

void Game::winGame(bool color) {
    std::cout << (color ? "Black" : "White") << " wins the game!\n";
    gameOver = true;
}

void Game::end(bool color) {
    winGame(color);
    std::cout << "Insert any key to exit...\n";
    std::cin.ignore();
    board.clear();
    activePieces.clear();
}