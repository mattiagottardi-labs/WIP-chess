#include "piece.h"
#include "queen.h"
#include "empty.h"
#include <iostream>

Queen::Queen(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {}

std::map<std::pair<int,int>,bool> Queen::scope(){
    std::map<std::pair<int,int>, bool> res;
    bool blocked1 = false;
    bool blocked2 = false;
    bool blocked3 = false;
    bool blocked4 = false;
    bool blockedH = false;
    bool blockedV = false;
    
    for(int i = 1; i < 8; i++){  // Start from 1 to skip current position
        // Diagonal moves (Bishop-like)
        if(isValid({position.first + i, position.second + i}, color) && !blocked1){
            res[{position.first + i, position.second + i}] = true;
        } else {
            blocked1 = true;
        }
        if(isValid({position.first - i, position.second - i}, color) && !blocked2){
            res[{position.first - i, position.second - i}] = true;
        } else {
            blocked2 = true;
        }
        if(isValid({position.first + i, position.second - i}, color) && !blocked3){
            res[{position.first + i, position.second - i}] = true; 
        } else {
            blocked3 = true;
        }
        if(isValid({position.first - i, position.second + i}, color) && !blocked4){
            res[{position.first - i, position.second + i}] = true;
        } else {
            blocked4 = true;
        }
    }
    
    // Straight moves (Rook-like)
    for(int i = 0; i < 8; i++){
        if(i != position.second && isValid({position.first, i}, this->color) && !blockedV){
            res[{position.first, i}] = true;
        } else if(i != position.second) {
            blockedV = true;
        }
        
        if(i != position.first && isValid({i, position.second}, this->color) && !blockedH){
            res[{i, position.second}] = true;
        } else if(i != position.first) {
            blockedH = true;
        }
    }
    
    return res;
}

std::string Queen::getName(){
    return name+(color ? "b" : "w");
}