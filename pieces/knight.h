#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"
#include <string>
#include <map>
#include <utility>

class Knight : public Piece {
    public:
        Knight(std::pair<int,int> pos, bool col, bool addToActive = true);
        
        std::map<std::pair<int,int>,bool> scope() override;

        std::string getName() override;
        
        const std::string name = "N";
};

#endif