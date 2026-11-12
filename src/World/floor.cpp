#include "floor.h"

Floor::Floor() : currRoom(nullptr){
    int startX = Constants::FLOOR_SIZE / 2;
    int startY = Constants::FLOOR_SIZE / 2;

    rooms[startY][startX] = std::make_unique<Room>(startX, startY);
    currRoom = rooms[startY][startX].get();
}

void Floor::update(float deltaTime){
    if(currRoom) currRoom->update(deltaTime);
}

void Floor::render(SDL_Renderer* renderer) {
    if (currRoom) currRoom->render(renderer);
}
