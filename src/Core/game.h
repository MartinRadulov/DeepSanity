#pragma once
#include <SDL2/SDL.h>
#include <memory>

class Player;
class Floor;

class Game{
private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::unique_ptr<Player> player;
    std::unique_ptr<Floor> floor;

public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void handleEvents();
    void update(float deltaTime);
    void render();
    void clean();

    bool running() const {return isRunning;}
};