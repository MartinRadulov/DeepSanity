#include "floor.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <queue>

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

        // diretiosn
        std::vector<Dir> availableDirs = getAvailableDirs(curr);

        // Pick random direction
        std::shuffle(availableDirs.begin(), availableDirs.end(), rng);
        for(const auto& d : availableDirs){
            int nx = curr->getX() + d.dx;
            int ny = curr->getY() + d.dy;

            // Chance for shape
            std::uniform_int_distribution<int> specialChance(1, 2);
            if(specialChance(rng) == 1){
                std::uniform_int_distribution<int> typeRoll(1, 3);
                int roll = typeRoll(rng);

                if(roll == 1){
                    roomPlaced = tryPlaceBig2x2(nx, ny, rng);
                } 
                else if(roll == 2){
                    roomPlaced = tryPlaceSkinny(nx, ny, d, rng);
                }
                else{
                    roomPlaced = tryPlaceL(nx, ny, rng);
                }
            }

            // NORMAL 1x1 (Fallback if no special room was placed)
            if(!roomPlaced){
                roomPlaced = tryPlaceNormal(nx, ny);
            }

            if(roomPlaced) break;
        }
    }

    placeManRooms(rng);
    placeMaybeRooms(rng);
    tryPlaceSecretRoom(rng);
}

std::vector<Dir> Floor::getAvailableDirs(const Room* room) const{
    if(isSkinnyH(room->getShape())){
        return { {-1, 0}, {1, 0} };
    }
    else if(isSkinnyV(room->getShape())){
        return { {0, -1}, {0, 1} };
    }
    return dirs;
}

bool Floor::tryPlaceBig2x2(int nx, int ny, std::mt19937& rng){
    std::vector<Dir> testCorners = TILES_2x2;
    std::shuffle(testCorners.begin(), testCorners.end(), rng);

    for(const auto& corner : testCorners){
        int ax = nx - corner.dx;
        int ay = ny - corner.dy;
        if(canFit(ax, ay, RoomShape::BIG_2x2, TILES_2x2)){
            placeRoom(ax, ay, RoomShape::BIG_2x2, TILES_2x2, RoomType::NORMAL);
            return true;
        }
    }
    return false;
}

bool Floor::tryPlaceL(int nx, int ny, std::mt19937& rng){
    std::vector<std::vector<Dir>> orient = L_ORIENTATIONS;
    std::shuffle(orient.begin(), orient.end(), rng);

    for(const auto& shapeTiles : orient){
        std::vector<Dir> testTiles = shapeTiles;
        std::shuffle(testTiles.begin(), testTiles.end(), rng);

        for(const auto& t : testTiles){
            int ax = nx - t.dx;
            int ay = ny - t.dy;
            if(canFit(ax, ay, RoomShape::L_SHAPE, shapeTiles)){
                placeRoom(ax, ay, RoomShape::L_SHAPE, shapeTiles, RoomType::NORMAL);
                return true;
            }
        }
    }
    return false;
}

bool Floor::tryPlaceSkinny(int nx, int ny, const Dir& d, std::mt19937& rng){
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
            if(d.dx == -1) ax = nx - 1;
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
        return true;
    }
    return false;
}

bool Floor::tryPlaceNormal(int nx, int ny){
    if(canFit(nx, ny, RoomShape::NORMAL_1x1, TILES_1x1)){
        placeRoom(nx, ny, RoomShape::NORMAL_1x1, TILES_1x1, RoomType::NORMAL);
        return true;
    }
    return false;
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

                if(isSkinnyH(shape) && d.dy != 0) return false;
                if(isSkinnyV(shape) && d.dx != 0) return false;

                if(isSkinnyH(neighShape) && d.dy != 0) return false;
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

    for(const auto& t : tiles){
        int cx = ax + t.dx;
        int cy = ay + t.dy;
        for(const auto& d : dirs){
            int nx = cx + d.dx;
            int ny = cy + d.dy;
            if(isValidCoord(nx, ny) && grid[ny][nx] != nullptr){
                Room* neigh = grid[ny][nx];
                if(neigh != ptr){
                    ptr->addDoor(cx, cy, d, neigh);
                    neigh->addDoor(nx, ny, {-d.dx, -d.dy}, ptr);
                }
            }
        }
    }
}

void Floor::placeManRooms(std::mt19937& rng){
    // 1. Boss room placed at maximum distance via BFS
    tryPlaceBossRoom(rng);

    // 2. Shop and Treasure placed at remaining dead-ends
    tryPlaceSpecialRoom(RoomType::SHOP, rng);
    tryPlaceSpecialRoom(RoomType::TREASURE, rng);
}

std::vector<std::vector<int>> Floor::calculateCellDistances() const{
    int startX = Constants::FLOOR_SIZE / 2;
    int startY = Constants::FLOOR_SIZE / 2;

    std::vector<std::vector<int>> dist(
        Constants::FLOOR_SIZE,
        std::vector<int>(Constants::FLOOR_SIZE, -1)
    );

    std::queue<std::pair<int, int>> q;
    dist[startY][startX] = 0;
    q.push({startX, startY});

    while(!q.empty()){
        auto [cx, cy] = q.front();
        q.pop();

        Room* curr = grid[cy][cx];

        for(const auto& d : dirs){
            int nx = cx + d.dx;
            int ny = cy + d.dy;

            if(!isValidCoord(nx, ny) || grid[ny][nx] == nullptr) continue;
            if(dist[ny][nx] != -1) continue;

            // Inside same room, or connected by a door!
            bool canMove = (grid[ny][nx] == curr) || curr->hasDoor(cx, cy, d);

            if(canMove){
                dist[ny][nx] = dist[cy][cx] + 1;
                q.push({nx, ny});
            }
        }
    }

    return dist;
}

bool Floor::tryPlaceBossRoom(std::mt19937& rng){
    auto dist = calculateCellDistances();

    int maxDist = -1;
    std::vector<std::pair<int, int>> bestSpots;

    for(int y = 0; y < Constants::FLOOR_SIZE; y++){
        for(int x = 0; x < Constants::FLOOR_SIZE; x++){
            if(grid[y][x] == nullptr || grid[y][x]->getType() != RoomType::NORMAL) continue;
            if(dist[y][x] == -1) continue;

            for(const auto& d : dirs){
                int bx = x + d.dx;
                int by = y + d.dy;

                if(canFit(bx, by, RoomShape::NORMAL_1x1, TILES_1x1)){
                    int stepDist = dist[y][x] + 1;

                    if(stepDist > maxDist){
                        maxDist = stepDist;
                        bestSpots.clear();
                        bestSpots.push_back({bx, by});
                    } else if(stepDist == maxDist){
                        bestSpots.push_back({bx, by});
                    }
                }
            }
        }
    }

    if(bestSpots.empty()) return false;

    std::uniform_int_distribution<size_t> pick(0, bestSpots.size() - 1);
    auto [bossX, bossY] = bestSpots[pick(rng)];

    placeRoom(bossX, bossY, RoomShape::NORMAL_1x1, TILES_1x1, RoomType::BOSS);
    return true;
}

bool Floor::tryPlaceSecretRoom(std::mt19937& rng){
    int maxNeighs = 0;
    std::vector<std::pair<int, int>> bestSpots;

    for(int y = 0; y < Constants::FLOOR_SIZE; y++){
        for(int x = 0; x < Constants::FLOOR_SIZE; x++){
            if(grid[y][x] != nullptr) continue;

            // 1. Check if this spot touches any illegal walls or Boss room
            bool validSpot = true;
            for(const auto& d : dirs){
                int nx = x + d.dx;
                int ny = y + d.dy;
                if(isValidCoord(nx, ny) && grid[ny][nx] != nullptr){
                    Room* neigh = grid[ny][nx];
                    RoomShape nShape = neigh->getShape();

                    // Cannot touch closed side walls of skinny rooms!
                    if(isSkinnyH(nShape) && d.dy != 0) { validSpot = false; break; }
                    if(isSkinnyV(nShape) && d.dx != 0) { validSpot = false; break; }

                    // Cannot touch Boss room!
                    if(neigh->getType() == RoomType::BOSS) { validSpot = false; break; }
                }
            }

            if(!validSpot) continue;

            // 2. Count neighbors and find the maximum
            int neighs = countNeighs(x, y);
            if(neighs > 0){
                if(neighs > maxNeighs){
                    maxNeighs = neighs;
                    bestSpots.clear();
                    bestSpots.push_back({x, y});
                } else if(neighs == maxNeighs){
                    bestSpots.push_back({x, y});
                }
            }
        }
    }

    if(bestSpots.empty()) return false;

    // Pick randomly among the spots with the most neighbors
    std::uniform_int_distribution<size_t> pick(0, bestSpots.size() - 1);
    auto [secretX, secretY] = bestSpots[pick(rng)];

    placeRoom(secretX, secretY, RoomShape::NORMAL_1x1, TILES_1x1, RoomType::SECRET);
    return true;
}

void Floor::placeMaybeRooms(std::mt19937& rng){
    std::vector<RoomType> maybe = {
        RoomType::CURSE,
        RoomType::LIBRARY,
        RoomType::SACRIFICE
    };

    std::uniform_int_distribution<int> roll(1, 100);

    for(RoomType type : maybe){
        int chance = 0;
        if(type == RoomType::CURSE) chance = 50;       // 50% chance
        else if(type == RoomType::LIBRARY) chance = 25; // 25% chance
        else if(type == RoomType::SACRIFICE) chance = 30; // 30% chance

        if(roll(rng) <= chance){
            tryPlaceSpecialRoom(type, rng);
        }
    }
}

bool Floor::tryPlaceSpecialRoom(RoomType type, std::mt19937& rng){
    int attempts = 0;
    std::uniform_int_distribution<size_t> roomDist(0, rooms.size() - 1);
    while(attempts++ < 100){
        Room* curr = rooms[roomDist(rng)].get();
        if(curr->getType() != RoomType::NORMAL) continue;

        std::vector<Dir> avDirs = getAvailableDirs(curr);
        std::shuffle(avDirs.begin(), avDirs.end(), rng);
        for(const auto& d : avDirs){
            int nx = curr->getX() + d.dx;
            int ny = curr->getY() + d.dy;
            if(canFit(nx, ny, RoomShape::NORMAL_1x1, TILES_1x1)){
                bool touchesBoss = false;
                for(const auto& checkDir : dirs){
                    int adjX = nx + checkDir.dx;
                    int adjY = ny + checkDir.dy;
                    if(isValidCoord(adjX, adjY) && grid[adjY][adjX] != nullptr){
                        if(grid[adjY][adjX]->getType() == RoomType::BOSS){
                            touchesBoss = true;
                            break;
                        }
                    }
                }
                if(touchesBoss) continue;

                placeRoom(nx, ny, RoomShape::NORMAL_1x1, TILES_1x1, type);
                return true;
            }
        }
    }
    return false;
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
                            } else if (grid[y][x]->getShape() == RoomShape::L_SHAPE) {
                                std::cout << "L ";
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

