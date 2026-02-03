#ifndef PAWN_H
#define PAWN_H

#include "piece.h"
#include <string>
#include <map>
#include <utility>

extern std::map<std::pair<int,int>, Piece*> board;
extern std::vector<Piece*> activePieces;

class Pawn : public Piece {
    public:
        int movecount{0};
        Pawn(std::pair<int,int> pos, bool col, bool addToActive = true);
        std::map<std::pair<int,int>,bool> scope() override;
        void move(std::pair<int,int> &k) override;
        void upgrade();
        std::string getName() override;
        const std::string name = "P";
};
#endif