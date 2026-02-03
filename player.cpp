#include <utility>
#include "player.h"
#include "pieces/piece.h"
#include <iostream>
#include <limits>

Player::Player(bool c) : color(c) {}

bool Player::turn(){
            std::cout << "Player " << (color ? "Black" : "White") << "'s turn. Enter piece position to move (x y): ";
            int x, y;
            std::cin >> x >> y;
            std::pair<int,int> pos = {x,y};
            if(board.find(pos) != board.end() && !board[pos]->isEmpty() && board[pos]->color == color 
            && board[pos]->scope().size() > 0){
                selectPiece(pos);
                std::cout << "Enter char to change piece or enter target position to move to (x y): ";
                int tx , ty;
                std::cin >> tx >> ty;
                std::pair<int,int> target = {tx,ty};
                if(board[pos]->scope()[target]){
                    if(!board[target]->isEmpty()){
                        board[target]->die();
                    }
                    board[pos]->move(target);
                    return false;
                } else {
                    std::cout << "Invalid move. Try again.\n";
                    if(std::cin.fail()){
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                    return true;  
                }
            } else {
                std::cout << "Invalid piece selection. Try again.\n";
                if(std::cin.fail()){
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                return true;
            }       
        }

void Player::selectPiece(std::pair<int,int> pos){
    board[{pos}]->showPossibleMoves();
}

bool Player::kingInCheck(bool color){
    for(auto &piece : activePieces){
        if(piece->getName()[0] == 'K' && piece->color == color){
            return piece->isInCheck({piece->position.first, piece->position.second});
        }
    }
    return false;
}