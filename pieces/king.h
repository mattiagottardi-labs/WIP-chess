#ifndef KING_H
#define KING_H

#include "piece.h"
#include <string>
#include <map>
#include <utility>
#include "rook.h"
#include "../game.h"
extern Game game;
class King : public Piece {
    public:
        King(std::pair<int,int> pos, bool col, bool addToActive = true);
        
        std::map<std::pair<int,int>,bool> scope() override;
        void move(std::pair<int,int> &k) override;
        bool canCastle(Rook* rook);
        void die() override;
        std::string getName() override;
        const std::string name = "K";
        int movecounter = 0;
        bool isInCheck(std::pair<int,int> pos) override;
};

#endif