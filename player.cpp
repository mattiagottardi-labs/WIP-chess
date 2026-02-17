#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <limits>
#include "player.h"
#include "gameState.h"
#include "piece.h"

std::pair<bool,std::pair<char,int>> val_getPos(){ //validates the input and returns the position
    char x;
    int y;
    std::cin >> x >> y;
    if(std::cin.fail() || y > 8 || y < 0 || x < 'a' || x > 'h'){
        std::cout << "bad input";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return {false, {'a',1}};
    }else{
        return {true, {x,y}};
    }
}

Piece* Player::selectPiece(const std::pair<char, int> pos) {
    if (!ga) return nullptr;
    auto it = ga->board.find(pos);
    if (it != ga->board.end() && it->second->color == color) {
        return it->second.get(); // return reference to the piece
    }
    return nullptr; // No piece found or piece belongs to the opponent
}


void Player::resign() {
    std::cout << (color ? "White" : "Black") << " resigns. " << (color ? "Black" : "White") << " wins!" << std::endl;
    exit(0); // End the game
}

void Player::promote(std::string newPiece) {
    // This function should be called when a pawn reaches the opposite end of the board
    // It should replace the pawn with the new piece specified by newPiece
    // For simplicity, we will just print a message here
    std::cout << "Pawn promoted to " << newPiece << "!" << std::endl;
}


void Player::movePiece(Piece* piece, std::pair<char, int> newPos) {
    // Store old position
    std::pair<char, int> oldPos = piece->pos;
    
    // Check for capture at destination
    auto it = ga->board.find(newPos);
    if (it != ga->board.end()) {
        // Capture enemy piece (erase it)
        ga->board.erase(newPos);
    }
    
    // Move the piece (transfer ownership from old to new position)
    std::unique_ptr<Piece> piecePtr = std::move(ga->board[oldPos]);
    ga->board.erase(oldPos);  // Remove from old position
    
    // Update piece's internal state
    piecePtr->pos = newPos;
    piecePtr->hasMoved = true;
    
    // Place at new position
    ga->board[newPos] = std::move(piecePtr);
    
    // Update king position if moving king
    if (piece->type == Piece::PieceType::King) {
        if (color) {
            ga->whiteKingPosition = newPos;
        } else {
            ga->blackKingPosition = newPos;
        }
    }
    
    // Update game state
    ga->updatePieces();
    ga->updateAttacking(this->color);
    ga->updateAttacking(!this->color);  // Update opponent's attacking too
}

bool Player::validateMove(Piece* piece, std::pair<char,int> canPos) {
    auto oldPos = piece->pos;

    // Save captured piece
    std::unique_ptr<Piece> captured = nullptr;
    auto captureIt = ga->board.find(canPos);
    if (captureIt != ga->board.end()) {
        captured = std::move(captureIt->second);
        ga->board.erase(captureIt);
    }

    // Safe find 
    auto pieceIt = ga->board.find(oldPos);
    if (pieceIt == ga->board.end()) return false; // ← guard against bad state

    std::unique_ptr<Piece> temp = std::move(pieceIt->second);
    ga->board.erase(pieceIt);
    temp->pos = canPos;
    ga->board[canPos] = std::move(temp);

    ga->updatePieces();
    ga->updateAttacking(true);
    ga->updateAttacking(false);
    bool inCheck = ga->isInCheck(color);

    // Undo
    auto undoIt = ga->board.find(canPos);
    std::unique_ptr<Piece> movedBack = std::move(undoIt->second);
    ga->board.erase(undoIt);
    movedBack->pos = oldPos;
    ga->board[oldPos] = std::move(movedBack);

    if (captured) ga->board[canPos] = std::move(captured);

    ga->updatePieces();
    ga->updateAttacking(true);
    ga->updateAttacking(false);

    return !inCheck;
}
std::vector<std::pair<char,int>> Player::getRealScope(Piece* piece) {
    std::vector<std::pair<char,int>> res;
    std::vector<std::pair<char,int>> res2;
    
    res = piece->checkfree_scope();
    // Get attack/movement scope (for most pieces, this is both)
     
    // Pawns: add forward movement (separate from diagonal attacks)
    if(piece->type == Piece::PieceType::Pawn) {
        int direction = color ? 1 : -1;  // white goes up (+1), black goes down (-1)
        // One square forward
        std::pair<char, int> oneForward = {piece->pos.first, piece->pos.second + direction};
        
        // Check boundaries and if square is empty
        if(oneForward.second >= 1 && oneForward.second <= 8 && ga->isEmpty(oneForward)) {
            res.push_back(oneForward);
            
            // Two squares forward (only if hasn't moved and one forward is clear)
            if(!piece->hasMoved) {
                std::pair<char, int> twoForward = {piece->pos.first, piece->pos.second + (2 * direction)};
                
                if(twoForward.second >= 1 && twoForward.second <= 8 && ga->isEmpty(twoForward)) {
                    res.push_back(twoForward);
                }
            }
        }
    }

  
    if(res.size() == 0){
        return {};
    }
    // Filter out moves that would put own king in check
    for(auto it : res) {
        if(validateMove(piece, it)) {
            res2.push_back(it);
        }
    }
    
    return res2;
}

/* PLAYER TURN LOGIC:

    player inserts a position, program must check:
    - the position inserted contains a piece of the correct color
    - The piece can move (scope > 0)
    the program then tells user the available moves for the piece

    player inserts a second position, program must check:
    - the position is valid 
    - the piece can move there

    the piece then moves
    attacked pieces must be updated
*/

Player::TurnResult Player::turn() {

    ga->updatePieces();

    if(isOutOfMoves() && ga->isInCheck(color)){
        return TurnResult::Checkmate;
    }
    if(isOutOfMoves() && !ga->isInCheck(color)){
        return TurnResult::Stalemate;
    }

    for (auto* piece : ga->whitePieces) {
    std::cout << "Piece: " << piece->pos.first << piece->pos.second << std::flush;
    auto moves = getRealScope(piece);
    std::cout << " -> " << moves.size() << " moves" << std::endl;
    }
    

    std::pair<char,int> pos;
    Piece* piece;

    std::cout << (color ? "White" : "Black") << " Player's turn.\n";
    std::string stat;
    std::cout<<"Insert re if you wish to resign: \n";
    std::cin >> stat;
    if(stat == "re"){
        return TurnResult::Resign;
    }else{
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // --- SELECT PIECE ---
    while (true) {
        auto input = val_getPos();
        if (!input.first) continue;

        pos = input.second;
        piece = selectPiece(pos);

        if (!piece) {
            std::cout << "No piece on that square\n";
            continue;
        }

        piece->scope = getRealScope(piece);

        if (!piece->scope.empty()) break;

        std::cout << "Invalid piece choice\n";
    }

    // --- CASTLING OPTIONS ---
    std::pair<char,int> leftCast = {'z',99};
    std::pair<char,int> rightCast = {'z',99};

    if (piece->type == Piece::PieceType::King) {

        int rank = color ? 1 : 8;

        if (canCastleLeft()) {
            std::cout << "You can castle left\n";
            leftCast = {'c', rank};
            piece->scope.push_back(leftCast);
        }

        if (canCastleRight()) {
            std::cout << "You can castle right\n";
            rightCast = {'g', rank};
            piece->scope.push_back(rightCast);
        }
    }

    // --- SHOW MOVES ---
    std::cout << "Possible moves:\n";
    for (auto m : piece->scope)
        std::cout << "(" << m.first << ", " << m.second << ")\n";

    // --- SELECT MOVE ---
    while (true) {
        auto input = val_getPos();
        if (!input.first) continue;

        pos = input.second;

        if (pos == leftCast) {
            castle(true);
            break;
        }

        if (pos == rightCast) {
            castle(false);
            break;
        }

        bool valid = false;
        for (auto m : piece->scope) {
            if (m == pos) {
                movePiece(piece, pos);
                valid = true;
                break;
            }
        }

        if (valid) break;
        std::cout << "Invalid movement\n";
    }

    ga->updateAttacking(color);
    return TurnResult::Continue;
}

bool Player::canCastleLeft() {
    // 1. Check rook exists and is unmoved
    auto rookPos = color ? std::make_pair('a', 1) : std::make_pair('a', 8);
    auto it = ga->board.find(rookPos);
    if (it == ga->board.end()) return false;

    Piece* leftRook = it->second.get();
    if (leftRook->type != Piece::PieceType::Rook || leftRook->hasMoved)
        return false;

    // 2. Check king exists and is unmoved
    auto kingPos = color ? ga->whiteKingPosition : ga->blackKingPosition;
    Piece* king = ga->board.at(kingPos).get();
    if (king->hasMoved) return false;

    // 3. Required empty squares
    char b = 'b', c = 'c', d = 'd';
    int rank = color ? 1 : 8;

    if (!ga->isEmpty({b, rank}) ||
        !ga->isEmpty({c, rank}) ||
        !ga->isEmpty({d, rank}))
        return false;

    // 4. King cannot pass through check
    if (!validateMove(king, {d, rank})) return false;
    if (!validateMove(king, {c, rank})) return false;

    return true;
}

bool Player::canCastleRight() {
    // 1. Check rook exists and is unmoved
    auto rookPos = color ? std::make_pair('h', 1) : std::make_pair('h', 8);
    auto it = ga->board.find(rookPos);
    if (it == ga->board.end()) return false;

    Piece* rightRook = it->second.get();
    if (rightRook->type != Piece::PieceType::Rook || rightRook->hasMoved)
        return false;

    // 2. Check king exists and is unmoved
    auto kingPos = color ? ga->whiteKingPosition : ga->blackKingPosition;
    Piece* king = ga->board.at(kingPos).get();
    if (king->hasMoved) return false;

    // 3. Required empty squares
    char f = 'f', g = 'g';
    int rank = color ? 1 : 8;

    if (!ga->isEmpty({f, rank}) ||
        !ga->isEmpty({g, rank}))
        return false;

    // 4. King cannot pass through check
    if (!validateMove(king, {f, rank})) return false;
    if (!validateMove(king, {g, rank})) return false;

    return true;
}

void Player::castle(bool left){
    char file = left ? 'c' : 'g';
    char rookfile = left ? 'a': 'h';
    int shift = left ? 1 : -1;
    int rank = color ? 1 : 8;
    auto it1 = ga->board.find({'e', rank});
    Piece* king = it1->second.get();
    movePiece(king, {file,rank});
    auto it2 = ga->board.find({rookfile, rank});
    Piece* rook = it2->second.get();
    movePiece(rook,{file + shift, rank});
}

bool Player::isOutOfMoves() {
    auto& pieces = color ? ga->whitePieces : ga->blackPieces;
    for (auto& p : pieces) {
        if (getRealScope(p).size() > 0){return false;} 
        else { std::cout << p->getName() << " is oom";}
    }
    std::cout << "oom";
    return true;
}
 