#pragma once
#include <SDL2/SDL.h>
#include "Math/vector.h"

class Player{
private:
    Vector2 position;
    int width;
    int height;
    float speed;

public:
    Player(int startX, int startY);
    ~Player();

    void update();
    void render(SDL_Renderer* renderer);
};