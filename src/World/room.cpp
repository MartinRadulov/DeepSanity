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