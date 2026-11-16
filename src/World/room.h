#pragma once
#include <SDL2/SDL.h>
#include "constants.h"
#include <vector>

enum class TileType{
    EMPTY,
    FLOOR,
    WALL,
    DOOR
};

enum class RoomType{
    START,
    NORMAL,
    BOSS,
    TREASURE,
    SHOP,
    SECRET,
    CURSE,
    LIBRARY,
    SACRIFICE
};

enum class RoomShape{
    NORMAL_1x1,
    HORIZONTAL_2x1,
    VERTICAL_1x2,
    BIG_2x2,
    L_SHAPE,
    SKINNY_H_1x1,
    SKINNY_V_1x1,
    SKINNY_H_2x1,
    SKINNY_V_1x2
};

struct Dir{ int dx, dy; };

class Room;

struct Door{
    int cellX, cellY;
    Dir dir;
    Room* nextRoom;
};

class Room{
private:
    TileType tiles[Constants::ROOM_Y][Constants::ROOM_X];
    int gridX;
    int gridY;
    RoomType type;
    RoomShape shape;
    std::vector<Door> doors;

    void initTiles();

public:
    Room(int gridX = 6, int gridY = 6, RoomType type = RoomType::NORMAL, RoomShape shape = RoomShape::NORMAL_1x1);
    ~Room() = default;

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    TileType getTile(int x, int y) const;

    RoomType getType() const { return type;}
    void setType(RoomType newType) { type = newType; }

    RoomShape getShape() const { return shape;}
    void setShape(RoomShape newShape) { shape = newShape; }

    int getX() const { return gridX; }
    int getY() const { return gridY; }

    void addDoor(int cellX, int cellY, Dir d, Room* target);
    bool hasDoor(int cellX, int cellY, Dir d) const;
    const std::vector<Door>& getDoors() const { return doors; }
};