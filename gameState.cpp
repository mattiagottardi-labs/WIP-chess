#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "gameState.h"
#include "player.h"
#include "piece.h"


void GameState::init() {
    std::cout << "Initializing game ..." << std::endl;
    
    // Initialize players first
    whitePlayer = std::make_unique<Player>(this, true);
    blackPlayer = std::make_unique<Player>(this, false);
    
    // White pieces (rank 1)
    board[{'a', 1}] = std::make_unique<Piece>(Piece::PieceType::Rook, true, std::make_pair('a', 1), this);
    board[{'b', 1}] = std::make_unique<Piece>(Piece::PieceType::Knight, true, std::make_pair('b', 1), this);
    board[{'c', 1}] = std::make_unique<Piece>(Piece::PieceType::Bishop, true, std::make_pair('c', 1), this);
    board[{'d', 1}] = std::make_unique<Piece>(Piece::PieceType::Queen, true, std::make_pair('d', 1), this);
    board[{'e', 1}] = std::make_unique<Piece>(Piece::PieceType::King, true, std::make_pair('e', 1), this);
    board[{'f', 1}] = std::make_unique<Piece>(Piece::PieceType::Bishop, true, std::make_pair('f', 1), this);
    board[{'g', 1}] = std::make_unique<Piece>(Piece::PieceType::Knight, true, std::make_pair('g', 1), this);
    board[{'h', 1}] = std::make_unique<Piece>(Piece::PieceType::Rook, true, std::make_pair('h', 1), this);

    // Pawns (rank 2 for white, rank 7 for black)
    for(int i = 0; i < 8; i++) {
        char file = static_cast<char>('a' + i);
        board[{file, 2}] = std::make_unique<Piece>(Piece::PieceType::Pawn, true, std::make_pair(file, 2), this);
        board[{file, 7}] = std::make_unique<Piece>(Piece::PieceType::Pawn, false, std::make_pair(file, 7), this);
    }

    // Black pieces (rank 8)
    board[{'a', 8}] = std::make_unique<Piece>(Piece::PieceType::Rook, false, std::make_pair('a', 8), this);
    board[{'b', 8}] = std::make_unique<Piece>(Piece::PieceType::Knight, false, std::make_pair('b', 8), this);
    board[{'c', 8}] = std::make_unique<Piece>(Piece::PieceType::Bishop, false, std::make_pair('c', 8), this);
    board[{'d', 8}] = std::make_unique<Piece>(Piece::PieceType::Queen, false, std::make_pair('d', 8), this);
    board[{'e', 8}] = std::make_unique<Piece>(Piece::PieceType::King, false, std::make_pair('e', 8), this);
    board[{'f', 8}] = std::make_unique<Piece>(Piece::PieceType::Bishop, false, std::make_pair('f', 8), this);
    board[{'g', 8}] = std::make_unique<Piece>(Piece::PieceType::Knight, false, std::make_pair('g', 8), this);
    board[{'h', 8}] = std::make_unique<Piece>(Piece::PieceType::Rook, false, std::make_pair('h', 8), this);
}

void GameState::printBoard() {
    std::cout << "  a b c d e f g h" << std::endl;
    for (int i = 8; i >= 1; --i) {
        std::cout << i << " ";
        for (char j = 'a'; j <= 'h'; ++j) {
            auto it = board.find({j, i});
            if (it != board.end()) {
                std::cout << it->second->getName() << " "; // Print the first letter of the piece name
            } else {
                std::cout << " . "; // Empty square
            }
        }
        std::cout << " " << i << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}

void GameState::updateAttacking(bool color) {
    if (color) {
        white_attacking.clear();
        for (auto& piece : whitePieces) {
            std::cout << "  checkfree_scope for white piece at: " 
                      << piece->pos.first << piece->pos.second << std::flush;
            auto scope = piece->checkfree_scope();
            std::cout << " -> done (" << scope.size() << " squares)" << std::endl;
            for (auto pos : scope) {
                white_attacking.push_back(pos);
            }
        }
    } else {
        black_attacking.clear();
        for (auto& piece : blackPieces) {
            std::cout << "  checkfree_scope for black piece at: " 
                      << piece->pos.first << piece->pos.second << std::flush;
            auto scope = piece->checkfree_scope();
            std::cout << " -> done (" << scope.size() << " squares)" << std::endl;
            for (auto pos : scope) {
                black_attacking.push_back(pos);
            }
        }
    }
}
void GameState::updatePieces(){ // this method is needed to update the pieces each of the two players has access
        whitePieces.clear();
        blackPieces.clear();
    for (int i = 8; i >= 1; --i) {
        for (char j = 'a'; j <= 'h'; ++j) {
            auto it = board.find({j, i});
            if (it != board.end()){
                if(it->second.get()->color){
                    whitePieces.push_back(it->second.get());
                }else {
                    blackPieces.push_back(it->second.get());
                } 
            } 
        }
    }
}

Piece* GameState::getPieceAt(std::pair<char,int> pos){
    auto it = board.find(pos);
    return (it != board.end()) ? it->second.get() : nullptr;
}

bool GameState::isInCheck(bool color){
    if(color){
        for(auto it : black_attacking){
            if( it == whiteKingPosition){
                return true;
            }
        }
        return false;
    }else{
        for(auto it : white_attacking){
            if( it == blackKingPosition){
                return true;
            }
        }
        return false;
    }
}