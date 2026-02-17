#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

class Player;
class Piece;

// Custom hash for std::pair (needed for unordered_map)
struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};


class GameState { //should keep state and be a reference for all other objects in game
    public:

    // GLOBAL VARIABLES, should be updated by gameState class and read by player and piece classes
    std::unordered_map<std::pair<char,int>, std::unique_ptr<Piece>, PairHash> board; //Pieces identified by position on board
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    std::vector<std::unique_ptr<Piece>> whitePieces;
    std::vector<std::unique_ptr<Piece>> blackPieces;
    std::vector<std::pair<char,int>> white_attacking;
    std::vector<std::pair<char,int>> black_attacking;
    std::pair<char,int> whiteKingPosition;
    std::pair<char,int> blackKingPosition;

    void init();
    void printBoard();

    //helpers:
    bool isEmpty(std::pair<char, int> pos) const {return board.find(pos) == board.end();}
    Piece* getPieceAt(std::pair<char, int> pos);
    void updatePieces(bool color);
    void updateAttacking(bool color); // this should be called at the end of every turn to update the attacking vectors for both players, it should iterate through all pieces and add their scope to the appropriate vector
    bool isInCheck(bool color);
};

#endif