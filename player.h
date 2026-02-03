#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include "pieces/piece.h"
#include <iostream>
class Player{
    public:
    Player();
    Player(bool c);
    public:
    bool color;
    bool kingInCheck(bool color);
    void selectPiece(std::pair<int,int> pos);
    bool turn();
};
#endif // PLAYER_H