#pragma once
#include <SDL2/SDL.h>

class Player{
private:
    int x;
    int y;
    int width;
    int height;

public:
    Player(int x, int y, int width, int height);
    ~Player();

    void update();
    void render(SDL_Renderer* renderer);
};