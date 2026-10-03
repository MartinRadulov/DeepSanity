#include <iostream>
#include <SDL2/SDL.h>

#include "Core/game.h"

int main(int argc, char* argv[]) {
    Game game;

    if(!game.init("DeepSanity", 800, 600)) return 1;

    while(game.running()){
        game.handleEvents();
        game.update();
        game.render();
    }

    game.clean();

    return 0;
}