#pragma once
#include <SDL2/SDL.h>
#include "Math/vector.h"

class Weapon{
protected:
    float cooldownTimer = 0.0f;

public:
    virtual ~Weapon() = default;

    bool canAttack() const { return cooldownTimer <= 0; }

    virtual void update(float deltaTime){
        if(cooldownTimer > 0.0f){
            cooldownTimer -= deltaTime;
        }
    }

    virtual void attack(Vector2 playerPos, Vector2 dir) {}
    virtual void render(SDL_Renderer* renderer) = 0;
};