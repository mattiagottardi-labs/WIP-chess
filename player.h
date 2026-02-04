#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include "pieces/piece.h"
#include <iostream>
class Player{

    public:
    int turncount= 1;
    Player();
    Player(bool c);
    public:
    bool color;
    bool kingInCheck();
    void selectPiece(std::pair<int,int> pos);
    bool turn();
};
#endif // PLAYER_H