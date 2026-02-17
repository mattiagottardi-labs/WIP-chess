#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <memory>
#include <utility>

class GameState; // Forward declaration
class Piece; // Forward declaration

std::pair<bool,std::pair<char,int>> val_getPos();

class Player { // Player only handles game logic, User will do the rest
    private:
    GameState* ga; // pointer to the game state, should be initialized in constructor and used to access board and pieces
    public:
    enum class TurnResult {
    Continue,
    Checkmate,
    Stalemate,
    Resign,
    Exit
};
    bool color;
    Player(GameState* gameState, bool playerColor) : ga(gameState), color(playerColor) {} // constructor to initialize game state pointer and player color 
    Piece* selectPiece(std::pair<char, int> pos);
    std::vector<std::pair<char, int>> getRealScope(Piece* piece);
    void promote(Piece * piece);
    void movePiece (Piece* piece, std::pair<char,int> newPos);
    bool validateMove(Piece* piece, std::pair<char,int> canPos);
    bool isInCheck();
    bool isOutOfMoves();
    void castle(bool left);
    bool canCastleLeft();
    bool canCastleRight();
    void resign();
    void promote(std::string newPiece);
    TurnResult turn(); // this is the main function that should be called in main, it should call selectPiece, getScope, and move in order, and also check for check and checkmate
};
#endif