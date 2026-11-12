#pragma once
#include <SDL2/SDL.h>
#include "Math/vector.h"
#include <vector>
#include "weapon.h"

class Sword : public Weapon{
private:
    Vector2 hitboxPos;
    float slashDur = 0.12f;
    float timer = 0.0f;
    bool isSwinging = false;
    float slashSize = 0.0f;
    Vector2 slashDir;
public:
    Sword() = default;
    ~Sword() override = default;

    void attack(Vector2 playerPos, float fireRate, float range, Vector2 dir);
    void render(SDL_Renderer* renderer) override;
    void update(float deltaTime, Vector2 playerPos) override;
    void moveSlash(Vector2 playerPos);
};