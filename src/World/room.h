#pragma once
#include <SDL2/SDL.h>
#include "constants.h"

enum class TileType{
    EMPTY,
    FLOOR,
    WALL,
    DOOR
};

class Room{
private:
    TileType tiles[Constants::ROOM_Y][Constants::ROOM_X];
    int gridX;
    int gridY;

    void initTiles();

public:
    Room(int gridX = 6, int gridY = 6);
    ~Room() = default;

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    TileType getTile(int x, int y) const;
};