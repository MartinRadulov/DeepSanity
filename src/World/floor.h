#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "room.h"
#include "constants.h"
#include <vector>

struct Dir{ int dx, dy; };

const std::vector<Dir> TILES_1x1 = { {0, 0} };
const std::vector<Dir> TILES_2x1 = { {0, 0}, {1, 0} };
const std::vector<Dir> TILES_1x2 = { {0, 0}, {0, 1} };
const std::vector<Dir> TILES_2x2 = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };


class Floor{
private:
    std::vector<std::unique_ptr<Room>> rooms;
    Room* grid[Constants::FLOOR_SIZE][Constants::FLOOR_SIZE];
    Room* currRoom;
    std::vector<Dir> dirs = {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0}
    };

public:
    Floor();
    ~Floor() = default;

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    bool isValidCoord(int x, int y) const;
    int countNeighs(int x, int y) const;
    Room* getRoom(int x, int y) const;

    void printFloor() const;
    void generateFloor();
    bool canFit(int ax, int ay, RoomShape shape, const std::vector<Dir>& tiles) const;
    void placeRoom(int ax, int ay, RoomShape shape, const std::vector<Dir>& tiles, RoomType type);

    static bool isSkinnyH(RoomShape shape);
    static bool isSkinnyV(RoomShape shape);
};