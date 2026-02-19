#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <string>
class GameState;
class Piece{
    private:
    GameState* gS;  // Pointer to game state
    public:
    enum class PieceType { Pawn, Knight, Bishop, Rook, Queen, King };\
    Piece(PieceType type, bool color, std::pair<char,int> pos, GameState* ga) : type(type), color(color), hasMoved(false), pos(pos), gS(ga){}

    std::pair<char, int> pos;
    PieceType type;
    bool color; // true for white, false for black
    std::vector<std::pair<char, int>> scope; // this is the vector of positions that the piece can move to, it should be updated every turn
    bool hasMoved; // this is for castling and pawn first move

    // METHODS:
    std::string getName();
    std::vector<std::pair<char,int>> checkfree_scope(); // used to compute the squared the opponent is attacking to figure out checks
};
#endif