#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"
#include <string>
#include <map>
#include <utility>

class Queen : public Piece {
    public:
        Queen(std::pair<int,int> pos, bool col, bool addToActive = true);
        std::map<std::pair<int,int>,bool> scope() override;
        std::string getName() override;
        const std::string name = "Q";
};

#endif