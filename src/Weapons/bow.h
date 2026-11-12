#pragma once
#include <SDL2/SDL.h>
#include "Math/vector.h"
#include <vector>
#include "weapon.h"

struct Arrow{
    Vector2 position;
    Vector2 velocity;
    float disTravel = 0.0f;
    float maxRange = 0.0f;
    bool isDead = false;
};

class Bow : public Weapon{
public:
    std::vector<Arrow> arrows;

    Bow() = default;
    ~Bow() override = default;

    void attack(Vector2 playerPos, Vector2 arrowVel, float fireRate, float range);
    void render(SDL_Renderer* renderer) override;
    void update(float deltaTime) override;

private:
    void cleanArrows();
    void movmentArrows(float deltaTime);
};