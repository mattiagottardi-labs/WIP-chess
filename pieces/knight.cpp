#include "knight.h"
#include "empty.h"
#include <iostream>

Knight::Knight(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {}

std::map<std::pair<int,int>,bool> Knight::scope(){

    std::map<std::pair<int,int>, bool> res;
    
    int moves[8][2] = {
        {1, 2}, {2, 1}, {-1, 2}, {-2, 1},
        {1, -2}, {2, -1}, {-1, -2}, {-2, -1}
    };
    for(auto &move : moves){
        int newX = position.first + move[0];
        int newY = position.second + move[1];
        if(isValid({newX, newY}, this->color)){
            res[{newX, newY}] = true;
        }
    }
    
    return res;
}
std::string Knight::getName() {
    return name+(color ? "b" : "w");
}