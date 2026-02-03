#include "empty.h"

Empty::Empty(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {}

bool Empty::isCapturable(bool color){
    return false;    
}

bool Empty::isEmpty(){
    return true;
}

std::string Empty::getName() {
    return "";
}