#ifndef ROOK_H
#define ROOK_H

#include "piece.h"
#include <string>
#include <map>
#include <utility>

class Rook : public Piece {
    public:
        Rook(std::pair<int,int> pos, bool col, bool addToActive = true);
        std::map<std::pair<int,int>,bool> scope() override;
        void move(std::pair<int,int> &k) override;
        bool canCastle();
        std::string getName() override;
        const std::string name = "R";
        int movecounter = 0;
};

#endif