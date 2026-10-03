#pragma once
#include <SDL2/SDL.h>

class Game{
private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() const {return isRunning;}
};