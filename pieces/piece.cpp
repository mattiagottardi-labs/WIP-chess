#include "piece.h"
#include "empty.h"
#include "king.h"
#include <iostream>
#include "../game.h"
std::map<std::pair<int,int>, Piece*> board;
std::vector<Piece*> activePieces;
extern Game game;
Piece::Piece() {}

Piece::Piece(std::pair<int,int> pos, bool col, bool addToActive) {
    this->position = pos;
    this->color = col;
    if(addToActive) activePieces.push_back(this);
}

std::map<std::pair<int,int>,bool> Piece::scope() {
    return {};
}

bool Piece::isEmpty() {
    return false;
}

std::string Piece::getName() {
    return this->name;
}

void Piece::move(std::pair<int,int> &k) {
    if(scope()[k] == true) {
        board[this->position] = new Empty(this->position, 2, false);
        this->position = k; 
        board[k] = this;
    }
    std::cout << "moved piece to: (" << position.first << "," << position.second << ")\n";
    game.printBoard();
}

bool Piece::isCapturable(bool color) {
    return (color != this->color) ? true : false;
}

void Piece::die() {
    for(auto it = activePieces.begin(); it != activePieces.end(); ++it) {
        if((*it)->position == this->position) {
            activePieces.erase(it);
            activePieces.shrink_to_fit();
            break;
        }
    }
}

bool isValid(std::pair<int,int> a, bool color) {
    if(a.first < 0 || a.second < 0 || a.first >= 8 || a.second >= 8) {
        return false;
    } else return ((board[{a.first, a.second}]->isEmpty() || board[{a.first, a.second}]->isCapturable(color))) ? true : false;
}

void Piece::showPossibleMoves() {
    auto moves = scope();
    std::cout << "Possible moves for " << this->getName() << " at (" << position.first << "," << position.second << "):\n";
    for(auto const& move : moves) {
        if(move.second) {
            std::cout << "(" << move.first.first << "," << move.first.second << ")\n";
        }
    }
}

bool Piece::isInCheck(std::pair<int,int> pos) {
    return false;
}