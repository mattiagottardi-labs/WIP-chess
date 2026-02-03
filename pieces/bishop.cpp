#include "bishop.h"
#include "empty.h"
#include <iostream>

Bishop::Bishop(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {}

std::map<std::pair<int,int>,bool> Bishop::scope(){
    std::map<std::pair<int,int>, bool> res;
    bool blocked1 = false;
    bool blocked2 = false;
    bool blocked3 = false;
    bool blocked4 = false;
    
    for(int i = 1; i < 8; i++){  // Start from 1 to skip current position
        // Diagonal: up-right
        if(isValid({position.first + i, position.second + i}, color) && !blocked1){
            res[{position.first + i, position.second + i}] = true;
        } else {
            blocked1 = true;
        }
        
        // Diagonal: down-left
        if(isValid({position.first - i, position.second - i}, color) && !blocked2){
            res[{position.first - i, position.second - i}] = true;
        } else {
            blocked2 = true;
        }
        
        // Diagonal: up-left
        if(isValid({position.first - i, position.second + i}, color) && !blocked3){
            res[{position.first - i, position.second + i}] = true; 
        } else {
            blocked3 = true;
        }
        
        // Diagonal: down-right
        if(isValid({position.first + i, position.second - i}, color) && !blocked4){
            res[{position.first + i, position.second - i}] = true;
        } else {
            blocked4 = true;
        }
    }
    
    return res;
}

std::string Bishop::getName() {
    return name + (color ? "b" : "w");
}