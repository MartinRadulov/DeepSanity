#include "room.h"

Room::Room(int gridX, int gridY, RoomType type, RoomShape shape) :
    gridX(gridX), gridY(gridY), type(type), shape(shape){
    initTiles();
}

void Room::initTiles(){
    for(int y = 0; y < Constants::ROOM_Y; y++){
        for(int x = 0; x < Constants::ROOM_X; x++){
            bool isEdge = (
                x == 0 || x == Constants::ROOM_X - 1 ||
                y == 0 || y == Constants::ROOM_Y - 1
            );
            if(isEdge) tiles[y][x] = TileType::WALL;
            else tiles[y][x] = TileType::FLOOR;
        }
    }
}

void Room::update(float deltaTime){

}

void Room::render(SDL_Renderer* renderer){
    for(int y = 0; y < Constants::ROOM_Y; y++){
        for(int x = 0; x < Constants::ROOM_X; x++){
            SDL_FRect tileRect = {
                static_cast<float>(x * Constants::TILE_SIZE),
                static_cast<float>(y * Constants::TILE_SIZE),
                static_cast<float>(Constants::TILE_SIZE),
                static_cast<float>(Constants::TILE_SIZE)
            };

            if(tiles[y][x] == TileType::WALL) SDL_SetRenderDrawColor(renderer, 75, 75, 90, 255);
            else if(tiles[y][x] == TileType::DOOR) SDL_SetRenderDrawColor(renderer, 160, 100, 50, 255);
            else if(tiles[y][x] == TileType::FLOOR) SDL_SetRenderDrawColor(renderer, 35, 35, 45, 255);

            SDL_RenderFillRectF(renderer, &tileRect);

            SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
            SDL_RenderDrawRectF(renderer, &tileRect);
        }
    }
}

TileType Room::getTile(int x, int y) const{
    if(x < 0 || x >= Constants::ROOM_X || y < 0 || y >= Constants::ROOM_Y) return TileType::WALL;
    return tiles[y][x];
}

bool Room::hasDoor(int cellX, int cellY, Dir d) const{
    for(const auto& door : doors){
        if(door.cellX == cellX && door.cellY == cellY &&
           door.dir.dx == d.dx && door.dir.dy == d.dy){
            return true;
        }
    }
    return false;
}

void Room::addDoor(int cellX, int cellY, Dir d, Room* target){
    if(hasDoor(cellX, cellY, d)) return;

    doors.push_back({cellX, cellY, d, target});

    int midX = Constants::ROOM_X / 2;
    int midY = Constants::ROOM_Y / 2;

    if(d.dy == -1){ // UP
        tiles[0][midX - 1] = TileType::DOOR;
        tiles[0][midX]     = TileType::DOOR;
    } else if(d.dy == 1){ // DOWN
        tiles[Constants::ROOM_Y - 1][midX - 1] = TileType::DOOR;
        tiles[Constants::ROOM_Y - 1][midX]     = TileType::DOOR;
    } else if(d.dx == -1){ // LEFT
        tiles[midY - 1][0] = TileType::DOOR;
        tiles[midY][0]     = TileType::DOOR;
    } else if(d.dx == 1){ // RIGHT
        tiles[midY - 1][Constants::ROOM_X - 1] = TileType::DOOR;
        tiles[midY][Constants::ROOM_X - 1]     = TileType::DOOR;
    }
}