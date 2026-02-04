#include <utility>
#include "player.h"
#include "pieces/piece.h"
#include <iostream>
#include <limits>

Player::Player(bool c) : color(c) {}

bool Player::turn() {
    std::cout << "\nturn count: " << this->turncount++ << "\n";

    if (kingInCheck()) {
        std::cout << "Your king is in check! You must move to cover it.\n";
    }

    std::cout << "Player " << (color ? "Black" : "White")
              << "'s turn. Enter piece position to move (x y): ";

    int x, y;
    std::cin >> x >> y;
    std::pair<int,int> pos = {x, y};

    // Validate piece selection
    auto it = board.find(pos);
    if (it != board.end() && it->second && !it->second->isEmpty()
        && it->second->color == color && !it->second->scope().empty()) {

        selectPiece(pos);

        std::cout << "Enter target position to move to (x y): ";
        int tx, ty;
        std::cin >> tx >> ty;
        std::pair<int,int> target = {tx, ty};
        auto piece = it->second;

        auto moves = piece->scope();

        // Check if target is in scope
        if (moves.count(target) && moves.at(target)) {

            // Capture if needed
            if (board.count(target) && board[target] && !board[target]->isEmpty()) {
                board[target]->die();
            }

            it->second->move(target);
            return false;
        }

        std::cout << "Invalid move. Try again.\n";
    } 
    else {
        std::cout << "Invalid piece selection. Try again.\n";
    }

    // Clear input if needed
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    return true;
}

void Player::selectPiece(std::pair<int,int> pos) {
    board[pos]->showPossibleMoves();

    std::cout << "Selected piece "
              << (board[pos]->canCover() ? "can" : "cannot")
              << " cover the king.\n";
}

bool Player::kingInCheck() {
    for (auto &piece : activePieces) {
        if (piece->getName()[0] == 'K' && piece->color == this->color) {
            return piece->isInCheck(piece->position);
        }
    }
    return false;
}