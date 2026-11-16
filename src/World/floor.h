#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "room.h"
#include "constants.h"
#include <random>

const std::vector<Dir> TILES_1x1 = { {0, 0} };
const std::vector<Dir> TILES_2x1 = { {0, 0}, {1, 0} };
const std::vector<Dir> TILES_1x2 = { {0, 0}, {0, 1} };
const std::vector<Dir> TILES_2x2 = { {0, 0}, {1, 0}, {0, 1}, {1, 1} };

const std::vector<std::vector<Dir>> L_ORIENTATIONS = {
    { {0, 0}, {0, 1}, {1, 1} }, // Missing Top-Right
    { {1, 0}, {0, 1}, {1, 1} }, // Missing Top-Left
    { {0, 0}, {1, 0}, {0, 1} }, // Missing Bottom-Right
    { {0, 0}, {1, 0}, {1, 1} }  // Missing Bottom-Left
};


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

    std::vector<Dir> getAvailableDirs(const Room* room) const;
    bool tryPlaceBig2x2(int nx, int ny, std::mt19937& rng);
    bool tryPlaceSkinny(int nx, int ny, const Dir& d, std::mt19937& rng);
    bool tryPlaceL(int nx, int ny, std::mt19937& rng);
    bool tryPlaceNormal(int nx, int ny);
    void placeManRooms(std::mt19937& rng);
    void placeMaybeRooms(std::mt19937& rng);
    bool tryPlaceSpecialRoom(RoomType type, std::mt19937& rng);
    std::vector<std::vector<int>> calculateCellDistances() const;
    bool tryPlaceBossRoom(std::mt19937& rng);

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