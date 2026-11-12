#include <iostream>
#include <SDL2/SDL.h>

#include "Core/game.h"
#include "constants.h"

int main(int argc, char* argv[]) {
    Game game;

    if(!game.init("Inferno1", Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT)) return 1;

    Uint32 lastTime = SDL_GetTicks();

    while(game.running()){
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        game.handleEvents();
        game.update(deltaTime);
        game.render();
    }

    game.clean();

    return 0;
}