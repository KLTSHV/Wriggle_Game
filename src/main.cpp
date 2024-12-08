#include "/Users/egorkoltysev/Desktop/PROG/Wriggle/include/Game.h"

int main() {
    Game game;
    if(game.startTheGame){
    game.gameClock.restart();
    game.run();
    
    return 0;}
}
