#ifndef EMPTY_H
#define EMPTY_H

#include "piece.h"
#include <utility>

class Empty : public Piece {
    public:
        Empty(std::pair<int,int> pos, bool col,bool addToActive = false);
        std::string getName() override;
        bool isCapturable(bool color) override;
        bool isEmpty() override;
};

#endif