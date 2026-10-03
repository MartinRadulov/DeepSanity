#pragma once
#include <SDL2/SDL.h>
#include <memory>

class Player;

class Game{
private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<Player> player;

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