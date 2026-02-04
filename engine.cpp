#include <map>
#include <string>
#include "pieces/piece.h"
#include "pieces/rook.h"
#include "pieces/knight.h"
#include "pieces/bishop.h"
#include "pieces/queen.h"
#include "pieces/king.h"
#include "pieces/pawn.h"
#include "pieces/empty.h"
#include "game.h"
#include "player.h"
#include <iostream>

int main(){
    std::cout << "Start chess Game Y/n\n";
    char ans;
    std::cin >> ans;
    if(ans != 'Y' && ans != 'y'){
        return 0;
    }
    game.initGame();
    game.printBoard();
    Player whitePlayer(false);
    Player blackPlayer(true);
    bool done=true;
    game.gameOver = false;
    while(game.gameOver == false){
        while(done){
            done = whitePlayer.turn();
        }
    done = true;
    game.printBoard();
        if(game.gameOver){
            break;
        } else {
            while(done){
            done = blackPlayer.turn();
        }}
    game.printBoard();
    done = true;
    }   
    return 0;
    }       
