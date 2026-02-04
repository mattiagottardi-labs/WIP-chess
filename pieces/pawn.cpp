#include "pawn.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include <iostream>
#include "empty.h"


Pawn::Pawn(std::pair<int,int> pos, bool col, bool addToActive) : Piece(pos, col, addToActive) {}

std::map<std::pair<int,int>,bool> Pawn::scope(){
    std::map<std::pair<int,int>, bool> res;
    if(this->color == 0){ // White pawns move up
        if(movecount == 0 && position.second < 6 && board[{position.first, position.second + 1}]->isEmpty() && board[{position.first, position.second + 2}]->isEmpty()){
            res[{position.first, position.second + 2}] = true;
        }
        // Forward move - check if empty
        if(position.second < 7 && board[{position.first, position.second + 1}]->isEmpty()){ 
            res[{position.first, position.second + 1}] = true;
        }
        // Diagonal captures
        if(position.first > 0 && position.second < 7 && 
           !board[{position.first - 1, position.second + 1}]->isEmpty() &&
           board[{position.first - 1, position.second + 1}]->isCapturable(0)){ 
            res[{position.first - 1, position.second + 1}] = true;
        }
        if(position.first < 7 && position.second < 7 && 
           !board[{position.first + 1, position.second + 1}]->isEmpty() &&
           board[{position.first + 1, position.second + 1}]->isCapturable(0)){
            res[{position.first + 1, position.second + 1}] = true;
        }
    } else { // Black pawns move down
        if(movecount == 0 && position.second > 1 && board[{position.first, position.second - 1}]->isEmpty() && board[{position.first, position.second - 2}]->isEmpty()){
            res[{position.first, position.second - 2}] = true;
        }
        // Forward move - check if empty
        if(position.second > 0 && board[{position.first, position.second - 1}]->isEmpty()){ 
            res[{position.first, position.second - 1}] = true;
        }
        // Diagonal captures
        if(position.first > 0 && position.second > 0 && 
           !board[{position.first - 1, position.second - 1}]->isEmpty() &&
           board[{position.first - 1, position.second - 1}]->isCapturable(1)){ 
            res[{position.first - 1, position.second - 1}] = true;
        }
        if(position.first < 7 && position.second > 0 && 
           !board[{position.first + 1, position.second - 1}]->isEmpty() &&
           board[{position.first + 1, position.second - 1}]->isCapturable(1)){
            res[{position.first + 1, position.second - 1}] = true;
        }
    }
    return res;
}

void Pawn::move(const std::pair<int,int> &k){
        if(scope()[k] == true) {
        board[this->position] = new Empty(this->position, 2, false);
        this->position = k; 
        board[k] = this;
    }
        std::cout << "moved pawn to: (" << position.first << "," << position.second << ")\n";
        
        // Check for promotion
        if((this->color == 0 && this->position.second == 7) || 
           (this->color == 1 && this->position.second == 0)){
            upgrade();
        }
    }


void Pawn::upgrade(){
    
    std::string choice;
    std::cout << "Pawn promotion! Choose a piece to upgrade to (Queen, Rook, Bishop, Knight): ";
    std::cin >> choice;
    
    Piece* newPiece = nullptr;
    if(choice == "Queen"){
        newPiece = new Queen(this->position, this->color);
    } else if(choice == "Rook"){
        newPiece = new Rook(this->position, this->color);
    } else if(choice == "Bishop"){
        newPiece = new Bishop(this->position, this->color);
    } else if(choice == "Knight"){
        newPiece = new Knight(this->position, this->color);
    } else {
        std::cout << "Invalid choice. Upgrading to Queen by default.\n";
        newPiece = new Queen(this->position, this->color);
    }
    
    board[this->position] = newPiece;
    this->die();
}

std::string Pawn::getName() {
    return name+(color ? "b" : "w");
}