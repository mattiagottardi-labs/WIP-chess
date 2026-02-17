#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

#include "piece.h"
#include "gameState.h"



std::vector<std::pair<char,int>> Piece::checkfree_scope(){ //theoretical scope to calculate threthened squares
    std::vector<std::pair<char,int>> res;
    switch (type){
        case PieceType::Pawn: {
            // Pawns attack diagonally (different from movement)
            int direction = color ? 1 : -1;  // white moves up, black down
            // Left diagonal attack
            if (pos.first > 'a' && pos.second + direction >= 1 && pos.second + direction <= 8) {
                res.push_back({char(pos.first - 1), pos.second + direction});
            }
            // Right diagonal attack
            if (pos.first < 'h' && pos.second + direction >= 1 && pos.second + direction <= 8) {
                res.push_back({char(pos.first + 1), pos.second + direction});
            }
            break;

        }
        case PieceType::King: {
            // Check all 8 surrounding squares (one square in each direction)
            for(int df = -1; df <= 1; df++) {       // file offset: -1, 0, +1
                for(int dr = -1; dr <= 1; dr++) {   // rank offset: -1, 0, +1
                    // Skip current position
                    if(df == 0 && dr == 0) continue;
                    
                    char newFile = pos.first + df;
                    int newRank = pos.second + dr;
                    
                    // Check bounds
                    if(newFile >= 'a' && newFile <= 'h' && 
                    newRank >= 1 && newRank <= 8) {
                        res.push_back({newFile, newRank});
                    }
                }
            }
            break;
        }
        case PieceType::Bishop:{
            std::vector<std::pair<int,int>> directions = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
            for(auto [df, dr] : directions) {      
                // Keep moving in this diagonal direction until hitting board edge or piece
                while(true) {
                    char currFile = pos.first;
                    int currRank = pos.second;
                    currFile += df;
                    currRank += dr;
                    
                    // Check if we're still on the board
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) {
                        break;  // Off the board
                    }
                    
                    std::pair<char, int> currPos = {currFile, currRank};
                    
                    // Check if square is occupied
                    auto it = gS->board.find(currPos);
                    if(it != gS->board.end()) {
                        //square is occupied!! check if capturable
                        Piece* pieceAtSquare = it->second.get();
                        if(pieceAtSquare->color == this->color) {
                        // Same color - blocked, can't move here
                            break;
                        } else {
                        // Opposite color - can capture, then stop
                        res.push_back(currPos);
                        break;
                    }
                    }
                    
                    // Square is empty - add it and continue
                    res.push_back(currPos);
                }
            }
                break;
            }

        case PieceType::Rook:{
            std::vector<std::pair<int,int>> directions = {{1,0}, {0,-1}, {-1,0}, {0,1}};
                
            for(auto [df,dr] : directions){
                while(true){
                    char currFile = this->pos.first;
                    int  currRank = this->pos.second;
                    currFile += df;
                    currRank += dr;

                    // Check if we're still on the board
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) {
                        break;  // Off the board
                    }

                    std::pair<char, int> currPos = {currFile, currRank};
                    auto it = gS->board.find(currPos);
                    // if it finds a piece it should consider it capturable but stop there
                    if(it != gS->board.end()) {
                        res.push_back(currPos);
                        break;
                    }
                    
                    // Square is empty - add it and continue
                    res.push_back(currPos);

                    }
                }
                break;
            }
            case PieceType::Knight:{
                std::vector<std::pair<int,int>> directions = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
                for(auto [df,dr] : directions){
                    char currFile = this->pos.first;
                    int  currRank = this->pos.second;
                    currFile += df;
                    currRank += dr;
                    // Check if we're still on the board
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) {
                        // Off the board
                    }else{
                    std::pair<char, int> currPos = {currFile, currRank};              
                    // Square is empty - add it and continue
                    res.push_back(currPos);
                    }
                    }
                    break;
                }
            case PieceType::Queen:{
                std::vector<std::pair<int,int>> directions = {{1,0}, {0,-1}, {-1,0}, {0,1}};
                for( auto [df,dr] : directions){
                while(true){
                    char currFile = this->pos.first;
                    int  currRank = this->pos.second;
                    currFile += df;
                    currRank += dr;

                    // Check if we're still on the board
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) {
                        break;  // Off the board
                    }

                    std::pair<char, int> currPos = {currFile, currRank};
                    auto it = gS->board.find(currPos);
                    // if it finds a piece it should consider it capturable but stop there
                    if(it != gS->board.end()) {
                        res.push_back(currPos);
                        break;
                    }
                    
                    // Square is empty - add it and continue
                    res.push_back(currPos);
                    }
                }
                    directions = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
                    for(auto [df,dr] : directions){
                    while(true){
                    char currFile = this->pos.first;
                    int  currRank = this->pos.second;
                    currFile += df;
                    currRank += dr;

                    // Check if we're still on the board
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) {
                        break;  // Off the board
                    }

                    std::pair<char, int> currPos = {currFile, currRank};
                    auto it = gS->board.find(currPos);
                    // if it finds a piece it should consider it capturable but stop there
                    if(it != gS->board.end()) {
                        res.push_back(currPos);
                        break;
                    }
                    
                    // Square is empty - add it and continue
                    res.push_back(currPos);

                    }
                    }
                    break;
                }
                default:{
                    break;
                }
                break;
            }
        return res;
        }
std::string Piece::getName(){
    switch (type){
        case PieceType::Bishop:{
            return std::string("B") + ((color) ? "w" : "b");
            break;
        }
        case PieceType::Rook:{
            return std::string("R") + ((color) ? "w" : "b");
            break;
        }
        case PieceType::Knight:{
            return std::string("N") + ((color) ? "w" : "b");
            break;
        }
        case PieceType::King:{
            return std::string("K") + ((color) ? "w" : "b");
            break;
        }
        case PieceType::Queen:{
            return std::string("Q") + ((color) ? "w" : "b");
            break;
        }
        case PieceType::Pawn:{
            return std::string("P") + ((color) ? "w" : "b");
            break;
        }
        default:{
            return std::string(".");
        }
        break;
    }
}