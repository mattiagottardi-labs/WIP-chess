#pragma once
#ifndef GAME_H
#define GAME_H
#include <map>
#include <utility>
class Game{
    public:
    bool gameOver = false;
    void initGame();
    void printBoard();
    void winGame(bool color);
    void end(bool color);
};
#endif // GAME_H