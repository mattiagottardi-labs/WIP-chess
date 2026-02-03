#ifndef PIECE_H
#define PIECE_H

#include <map>
#include <vector>
#include <utility>   // std::pair
#include <iostream>  // std::cout (optional, but you use it in move())

class Piece;  // forward declaration

// Global containers (defined in piece.cpp)
extern std::map<std::pair<int,int>, Piece*> board;
extern std::vector<Piece*> activePieces;

// Utility function (defined in piece.cpp)
bool isValid(std::pair<int,int> a, bool color);

class Piece {
public:

    Piece();
    Piece(std::pair<int,int> pos, bool col, bool addToActive = true);
    std::string name{};
    std::pair<int,int> position{};
    bool color{false};
    virtual void showPossibleMoves();
    virtual ~Piece() = default;
    virtual std::string getName();
    virtual std::map<std::pair<int,int>, bool> scope();
    virtual bool isInCheck(std::pair<int,int> pos);
    virtual void die();
    virtual bool isEmpty();
    virtual void move(std::pair<int,int> &k);
    virtual bool isCapturable(bool color);
};

#endif // PIECE_H
