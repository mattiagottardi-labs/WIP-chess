#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"
#include <string>
#include <map>
#include <utility>

class Bishop : public Piece {
    public:
        Bishop(std::pair<int,int> pos, bool col, bool addToActive = true);
        std::string getName() override;
        std::map<std::pair<int,int>,bool> scope() override;
        
        const std::string name = "B";
};

#endif