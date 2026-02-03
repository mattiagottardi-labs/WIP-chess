#include "king.h"
#include "empty.h"
#include <iostream>
#include "../game.h"
#include "piece.h"
#include "rook.h"
extern Game game;

King::King(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {
    movecounter = 0;
}

std::map<std::pair<int,int>,bool> King::scope(){
    std::map<std::pair<int,int>, bool> res;
    if(color == 1){
        // black king castling logic
        if(canCastle(dynamic_cast<Rook*>(board[{7,7}]))){
            if(board[{5,7}]->isEmpty() && board[{6,7}]->isEmpty()){
                res[{6,7}] = true;
            }
        }
        if(canCastle(dynamic_cast<Rook*>(board[{0,7}]))){
            if(board[{1,7}]->isEmpty() && board[{2,7}]->isEmpty() && board[{3,7}]->isEmpty()){
                res[{2,7}] = true;
            }
        }
    }else{
        //white king castling logic
        if(canCastle(dynamic_cast<Rook*>(board[{7,0}]))){
            if(board[{5,0}]->isEmpty() && board[{6,0}]->isEmpty()){
                res[{6,0}] = true;
            }
        }
        if(canCastle(dynamic_cast<Rook*>(board[{0,0}]))){
            if(board[{1,0}]->isEmpty() && board[{2,0}]->isEmpty() && board[{3,0}]->isEmpty()){
                res[{2,0}] = true;
            }
        }
    }
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue; // Skip current position
            
            int newX = position.first + i;
            int newY = position.second + j;
            
            if(isValid({newX, newY}, this->color)){
                res[{newX, newY}] = true;
            }
        }
    }
    
    return res;
}

void King::move(std::pair<int,int> &k){
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
    if(movecounter != 0) return false;
    if(!rook->canCastle()) return false;
    return true;
}
bool King::isInCheck(std::pair<int,int> pos){
     for(auto &piece : activePieces){
        if(piece->scope()[pos] == true && piece->color != this->color){
            return true;
        }
    }
    return false;
}
void King::die() {
    Piece::die();
    game.end(!color);
}