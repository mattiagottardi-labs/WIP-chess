#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

#include "piece.h"
#include "gameState.h"



std::vector<std::pair<char,int>> Piece::checkfree_scope(){
    std::vector<std::pair<char,int>> res;
    switch (type){
        case PieceType::Pawn: {
            int direction = color ? 1 : -1;
            if (pos.first > 'a' && pos.second + direction >= 1 && pos.second + direction <= 8)
                res.push_back({char(pos.first - 1), pos.second + direction});
            if (pos.first < 'h' && pos.second + direction >= 1 && pos.second + direction <= 8)
                res.push_back({char(pos.first + 1), pos.second + direction});
            break;
        }
        case PieceType::King: {
            for(int df = -1; df <= 1; df++) {
                for(int dr = -1; dr <= 1; dr++) {
                    if(df == 0 && dr == 0) continue;
                    char newFile = pos.first + df;
                    int newRank = pos.second + dr;
                    if(newFile >= 'a' && newFile <= 'h' && newRank >= 1 && newRank <= 8)
                        res.push_back({newFile, newRank});
                }
            }
            break;
        }
        case PieceType::Bishop: {
            std::vector<std::pair<int,int>> directions = {{1,1},{1,-1},{-1,1},{-1,-1}};
            for(auto [df, dr] : directions) {
                char currFile = pos.first;
                int  currRank = pos.second;
                while(true) {
                    currFile += df;
                    currRank += dr;
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) break;
                    res.push_back({currFile, currRank});
                    if(gS->board.find({currFile, currRank}) != gS->board.end()) break; // blocked
                }
            }
            break;
        }
        case PieceType::Rook: {
            std::vector<std::pair<int,int>> directions = {{1,0},{0,-1},{-1,0},{0,1}};
            for(auto [df, dr] : directions) {
                char currFile = pos.first;
                int  currRank = pos.second;
                while(true) {
                    currFile += df;
                    currRank += dr;
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) break;
                    res.push_back({currFile, currRank});
                    if(gS->board.find({currFile, currRank}) != gS->board.end()) break; // blocked
                }
            }
            break;
        }
        case PieceType::Knight: {
            std::vector<std::pair<int,int>> directions = {{2,1},{2,-1},{-2,1},{-2,-1},{1,2},{1,-2},{-1,2},{-1,-2}};
            for(auto [df, dr] : directions) {
                char currFile = pos.first + df;
                int  currRank = pos.second + dr;
                if(currFile >= 'a' && currFile <= 'h' && currRank >= 1 && currRank <= 8)
                    res.push_back({currFile, currRank});
            }
            break;
        }
        case PieceType::Queen: {
            std::vector<std::pair<int,int>> directions = {{1,0},{0,-1},{-1,0},{0,1},{1,1},{1,-1},{-1,1},{-1,-1}};
            for(auto [df, dr] : directions) {
                char currFile = pos.first;
                int  currRank = pos.second;
                while(true) {
                    currFile += df;
                    currRank += dr;
                    if(currFile < 'a' || currFile > 'h' || currRank < 1 || currRank > 8) break;
                    res.push_back({currFile, currRank});
                    if(gS->board.find({currFile, currRank}) != gS->board.end()) break; // blocked
                }
            }
            break;
        }
        default: break;
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