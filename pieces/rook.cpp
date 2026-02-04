#include "rook.h"
#include "empty.h"
#include <iostream>

Rook::Rook(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {
    movecounter = 0;
}

std::map<std::pair<int,int>, bool> Rook::scope() {
    std::map<std::pair<int,int>, bool> res;
    // Directions: right, left, up, down
    const int dirs[4][2] = {
        {1, 0},   // right
        {-1, 0},  // left
        {0, 1},   // up
        {0, -1}   // down
    };
    for (auto &d : dirs) {
        int x = position.first;
        int y = position.second;

        while (true) {
            x += d[0];
            y += d[1];
            // Out of board
            if (x < 0 || y < 0 || x >= 8 || y >= 8)
                break;
            Piece* p = board[{x,y}];
            if (p->isEmpty()) {
                res[{x,y}] = true;   // empty square → can move
                continue;            // keep going
            }
            if (p->color != this->color) {
                res[{x,y}] = true;   // enemy piece → can capture
            }
            break; // stop after hitting any piece
        }
    }

    return res;
}

std::string Rook::getName() {
    return name+(color ? "b" : "w");
}

void Rook::move(const std::pair<int,int> &k) {
    if(scope()[k] == true) {
        board[this->position] = new Empty(this->position, 2, false);
        this->position = k; 
        board[k] = this;
        std::cout << "moved piece to: (" << position.first << "," << position.second << ")\n";
        movecounter++;
        scope().clear(); // Clear cached moves after moving
    }
}

bool Rook::canCastle() {
    return (movecounter == 0);
}