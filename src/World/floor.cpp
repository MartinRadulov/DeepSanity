#include "floor.h"
#include <iostream>
#include <random>
#include <algorithm>

Floor::Floor() : currRoom(nullptr){
    for(int i = 0; i < 10; i++){
        generateFloor();
        printFloor();
        std::cout << std::endl << std::endl;
    }
}

void Floor::update(float deltaTime){
    if(currRoom) currRoom->update(deltaTime);
}

void Floor::render(SDL_Renderer* renderer) {
    if (currRoom) currRoom->render(renderer);
}

bool Floor::isValidCoord(int x, int y) const{
    return x >= 0 && x < Constants::FLOOR_SIZE && y >= 0 && y < Constants::FLOOR_SIZE;
}

int Floor::countNeighs(int x, int y) const{
    int count = 0;
    if(isValidCoord(x, y - 1) && grid[y - 1][x] != nullptr) count++;
    if(isValidCoord(x, y + 1) && grid[y + 1][x] != nullptr) count++;
    if(isValidCoord(x - 1, y) && grid[y][x - 1] != nullptr) count++;
    if(isValidCoord(x + 1, y) && grid[y][x + 1] != nullptr) count++;
    return count;
}

Room* Floor::getRoom(int x, int y) const{
    if(!isValidCoord(x, y)) return nullptr;
    return grid[y][x];
}

void Floor::generateFloor(){
    //Clearing everything
    rooms.clear();
    for(int y = 0; y < Constants::FLOOR_SIZE; y++){
        for(int x = 0; x < Constants::FLOOR_SIZE; x++){
            grid[y][x] = nullptr;
        }
    }

    //Initialize the starting room
    int startX = Constants::FLOOR_SIZE / 2;
    int startY = Constants::FLOOR_SIZE / 2;
    auto startRoom = std::make_unique<Room>(startX, startY, RoomType::START, RoomShape::NORMAL_1x1);
    currRoom = startRoom.get();
    rooms.push_back(std::move(startRoom));
    grid[startY][startX] = currRoom;

    //Start the random engine
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> roomCount(10, 13);
    int target = roomCount(rng);
    std::uniform_int_distribution<int> shapeDist(1, 8);

    //Generating cycle
    int attempts = 0;
    while(rooms.size() < target && attempts++ < 1000){
        //Pick a random room
        std::uniform_int_distribution<size_t> roomDist(0, rooms.size() - 1);
        size_t randI = roomDist(rng);
        Room* curr = rooms[randI].get();

        bool roomPlaced = false;

        // Determine which directions this room can branch into
        std::vector<Dir> availableDirs;
        if(isSkinnyH(curr->getShape())){
            availableDirs = { {-1, 0}, {1, 0} };
        } else if(isSkinnyV(curr->getShape())){
            availableDirs = { {0, -1}, {0, 1} };
        } else {
            availableDirs = dirs;
        }

        // Pick a random direction
        std::shuffle(availableDirs.begin(), availableDirs.end(), rng);
        for(const auto& d : availableDirs){
            int nx = curr->getX() + d.dx;
            int ny = curr->getY() + d.dy;

            // Chance for a special shape (1 in 2 for testing)
            std::uniform_int_distribution<int> specialChance(1, 2);
            if(specialChance(rng) == 1){
                std::uniform_int_distribution<int> typeRoll(1, 2); // 1 = Big 2x2, 2 = Skinny

                // --- BIG 2x2 ROOM ---
                if(typeRoll(rng) == 1){
                    std::vector<Dir> testCorners = TILES_2x2;
                    std::shuffle(testCorners.begin(), testCorners.end(), rng);

                    for(const auto& corner : testCorners){
                        int ax = nx - corner.dx;
                        int ay = ny - corner.dy;
                        if(canFit(ax, ay, RoomShape::BIG_2x2, TILES_2x2)){
                            placeRoom(ax, ay, RoomShape::BIG_2x2, TILES_2x2, RoomType::NORMAL);
                            roomPlaced = true;
                            break;
                        }
                    }
                }
                // --- SKINNY ROOM ---
                else{
                    std::uniform_int_distribution<int> lengthRoll(1, 2);
                    bool isLong = (lengthRoll(rng) == 2);

                    RoomShape skinnyShape;
                    std::vector<Dir> skinnyTiles;
                    int ax = nx;
                    int ay = ny;

                    if(d.dx != 0){ // Horizontal (Left or Right)
                        if(isLong){
                            skinnyShape = RoomShape::SKINNY_H_2x1;
                            skinnyTiles = TILES_2x1;
                            if(d.dx == -1) ax = nx - 1; //why not - 2
                        } 
                        else{
                            skinnyShape = RoomShape::SKINNY_H_1x1;
                            skinnyTiles = TILES_1x1;
                        }
                    }
                    else{ // Vertical (Up or Down)
                        if(isLong){
                            skinnyShape = RoomShape::SKINNY_V_1x2;
                            skinnyTiles = TILES_1x2;
                            if(d.dy == -1) ay = ny - 1;
                        }
                        else{
                            skinnyShape = RoomShape::SKINNY_V_1x1;
                            skinnyTiles = TILES_1x1;
                        }
                    }

                    if(canFit(ax, ay, skinnyShape, skinnyTiles)){
                        placeRoom(ax, ay, skinnyShape, skinnyTiles, RoomType::NORMAL);
                        roomPlaced = true;
                    }
                }
            }

            // NORMAL 1x1 (Fallback if no special room was placed)
            if(!roomPlaced && canFit(nx, ny, RoomShape::NORMAL_1x1, TILES_1x1)){
                placeRoom(nx, ny, RoomShape::NORMAL_1x1, TILES_1x1, RoomType::NORMAL);
                roomPlaced = true;
            }

            if(roomPlaced) break;
        }
    }
}

bool Floor::isSkinnyH(RoomShape shape){
    return shape == RoomShape::SKINNY_H_1x1 || shape == RoomShape::SKINNY_H_2x1;
}

bool Floor::isSkinnyV(RoomShape shape){
    return shape == RoomShape::SKINNY_V_1x1 || shape == RoomShape::SKINNY_V_1x2;
}

bool Floor::canFit(int ax, int ay, RoomShape shape, const std::vector<Dir>& tiles) const{
    for(const auto& t : tiles){
        int tx = ax + t.dx;
        int ty = ay + t.dy;
        if(!isValidCoord(tx, ty) || grid[ty][tx] != nullptr) return false;
    }

    // Check every neighbor around the new room
    for(const auto& t : tiles){
        int tx = ax + t.dx;
        int ty = ay + t.dy;

        for(const auto& d : dirs){
            int nx = tx + d.dx;
            int ny = ty + d.dy;

            if(isValidCoord(nx, ny) && grid[ny][nx] != nullptr){
                Room* neighbor = grid[ny][nx];
                RoomShape neighShape = neighbor->getShape();

                // If NEW room is horizontal skinny -> cannot have neighbors above/below!
                if(isSkinnyH(shape) && d.dy != 0) return false;

                // If NEW room is vertical skinny -> cannot have neighbors left/right!
                if(isSkinnyV(shape) && d.dx != 0) return false;

                // If NEIGHBOR is horizontal skinny -> cannot touch its top/bottom wall!
                if(isSkinnyH(neighShape) && d.dy != 0) return false;

                // If NEIGHBOR is vertical skinny -> cannot touch its side wall!
                if(isSkinnyV(neighShape) && d.dx != 0) return false;
            }
        }
    }

    if(shape == RoomShape::NORMAL_1x1) return countNeighs(ax, ay) == 1;

    return true;
}

void Floor::placeRoom(int ax, int ay, RoomShape shape, const std::vector<Dir>& tiles, RoomType type){
    auto newRoom = std::make_unique<Room>(ax, ay, type, shape);
    Room* ptr = newRoom.get();
    rooms.push_back(std::move(newRoom));
    for(const auto& t : tiles){
        grid[ay + t.dy][ax + t.dx] = ptr;
    }
}

void Floor::printFloor() const {
        std::cout << "\n--- FLOOR MAP ---\n";
        for (int y = 0; y < Constants::FLOOR_SIZE; y++) {
            for (int x = 0; x < Constants::FLOOR_SIZE; x++) {
                if (grid[y][x] == nullptr) {
                    std::cout << ". ";
                } else {
                    switch (grid[y][x]->getType()) {
                        case RoomType::START:     std::cout << "S "; break;
                        case RoomType::NORMAL:
                            if (grid[y][x]->getShape() == RoomShape::BIG_2x2) {
                                std::cout << "2 ";
                            } else if (isSkinnyH(grid[y][x]->getShape())) {
                                std::cout << "= ";
                            } else if (isSkinnyV(grid[y][x]->getShape())) {
                                std::cout << "| ";
                            } else {
                                std::cout << "N ";
                            }
                            break;
                        case RoomType::BOSS:      std::cout << "B "; break;
                        case RoomType::TREASURE:  std::cout << "T "; break;
                        case RoomType::SHOP:      std::cout << "$ "; break;
                        case RoomType::SECRET:    std::cout << "? "; break;
                        case RoomType::CURSE:     std::cout << "C "; break;
                        case RoomType::LIBRARY:   std::cout << "L "; break;
                        case RoomType::SACRIFICE: std::cout << "X "; break;
                        default:                  std::cout << "N "; break;
                    }
                }
            }
            std::cout << "\n";
        }
        std::cout << "-----------------\n\n";
    }

