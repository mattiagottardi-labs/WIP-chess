#include "rook.h"
#include "empty.h"
#include <iostream>

Rook::Rook(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {
    // Base class constructor already handles position, color, and adding to activePieces
    movecounter = 0;
}

std::map<std::pair<int,int>,bool> Rook::scope() {
    std::map<std::pair<int,int>, bool> res;
    bool blockedH = false;
    bool blockedV = false;
    
    for(int i = 0; i < 8; i++) {
        if(isValid({i, position.second}, this->color) && !blockedH) {
            res[{i, position.second}] = true;
        } else {
            blockedH = true;
        }
        
        if(isValid({position.first, i}, this->color) && !blockedV) {
            res[{position.first, i}] = true;
        } else {
            blockedV = true;
        }
    }
    return res;
}

std::string Rook::getName() {
    return name+(color ? "b" : "w");
}

void Rook::move(std::pair<int,int> &k) {
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