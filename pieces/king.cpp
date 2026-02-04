#include "king.h"
#include "empty.h"
#include <iostream>
#include "../game.h"
#include "piece.h"
#include "rook.h"
extern Game game;

King::King(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {
    movecounter = 0;
    kings[col] = this;
}

std::map<std::pair<int,int>,bool> King::scope(){
    std::map<std::pair<int,int>, bool> res;
    if(color == 1){
        // black king castling logic
        if(canCastle(dynamic_cast<Rook*>(board[{7,7}]))){
            if(board[{5,7}]->isEmpty() && board[{6,7}]->isEmpty()){
                res[{6,7}];
            }
        }
        if(canCastle(dynamic_cast<Rook*>(board[{0,7}]))){
            if(board[{1,7}]->isEmpty() && board[{2,7}]->isEmpty() && board[{3,7}]->isEmpty()){
                res[{2,7}];
            }
        }
    }else{
        //white king castling logic
        if(canCastle(dynamic_cast<Rook*>(board[{7,0}]))){
            if(board[{5,0}]->isEmpty() && board[{6,0}]->isEmpty() && !isInCheck({5,0}) && !isInCheck({6,0})){
                res[{6,0}];
            }
        } 
        if(canCastle(dynamic_cast<Rook*>(board[{0,0}]))){
            if(board[{1,0}]->isEmpty() && board[{2,0}]->isEmpty() && board[{3,0}]->isEmpty() && !isInCheck({3,0}) && !isInCheck({2,0})&& !isInCheck({1,0})){
                res[{2,0}];
            }
        }
    }
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue; // Skip current position
            
            int newX = position.first + i;
            int newY = position.second + j;
            
            if(isValid({newX, newY}, this->color) && !isInCheck({newX, newY})){
                res[{newX, newY}] = true;
            }
        }
    }
    
    return res;
}

void King::move(const std::pair<int,int> &k){
    if(board[k]->getName()[0] == 'R' && canCastle(dynamic_cast<Rook*>(board[k]))){
        Rook* rook = dynamic_cast<Rook*>(board[k]);
        if(rook->position.first == 0){ // Queenside castling
            board[position] = new Empty(position, 2, false);
            board[{2, position.second}] = this;
            board[{3, position.second}] = rook;
            board[rook->position] = new Empty(rook->position, 2, false);
            rook->position = {3, position.second};
        } else if(rook->position.first == 7){ // Kingside castling
            board[position] = new Empty(position, 2, false);
            board[{6, position.second}] = this;
            board[{5, position.second}] = rook;
            board[rook->position] = new Empty(rook->position, 2, false);
            rook->position = {5, position.second};
        }
        rook->movecounter++;
    } else{
    if(scope()[k] && !isInCheck(k)){ // Check if the move is valid and doesn't put the king in check
        board[this->position] = new Empty(this->position, 2, false);
        this->position = k;
        std::cout << "moved King to: (" << position.first << "," << position.second << ")\n";
        board[k] = this;
        movecounter++;
    }}
}

std::string King::getName() {
    return name+(color ? "b" : "w");
}

bool King::canCastle(Rook* rook) {
    if(rook == nullptr || !rook->canCastle() || rook->color != this->color || this->movecounter != 0 //move counter must be 0 for both king and rook
    || isInCheck(this->position) || isInCheck({(rook->position.first == 0) ? 2 : 6, this->position.second}) //all positions between king and rook must not be in check
    || isInCheck({(rook->position.first == 0) ? 3 : 5, this->position.second})  || isInCheck({(rook->position.first == 0) ? 4 : 6, this->position.second})){
        return false;
    } else
    return true;
}
bool King::isInCheck(std::pair<int,int> pos) {
    for (auto &piece : activePieces) {
        // Avoid recursive king–king scope calls
        if (piece->getName()[0] == 'K') continue;
        if (piece->color == this->color) continue;

        auto moves = piece->scope();
        if (moves.count(pos) && moves.at(pos)) {
            return true;
        }
    }
    return false;
}

void King::die() {
    Piece::die();
    game.end(!color);
}