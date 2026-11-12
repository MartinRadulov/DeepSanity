#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "room.h"
#include "constants.h"

class Floor{
private:
    std::unique_ptr<Room> rooms[Constants::FLOOR_SIZE][Constants::FLOOR_SIZE];
    Room* currRoom;

public:
    Floor();
    ~Floor() = default;

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
};