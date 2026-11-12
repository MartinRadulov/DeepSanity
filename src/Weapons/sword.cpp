#include "sword.h"
#include "constants.h"

void Sword::attack(Vector2 playerPos, float fireRate, float range){
    if(!canAttack()) return;

    hitboxPos = playerPos;
    slashSize = range;
    isSwinging = true;
    timer = slashDur;
    cooldownTimer = fireRate;
}

void Sword::update(float deltaTime){
    Weapon::update(deltaTime);
    
    if(isSwinging){
        timer -= deltaTime;
        if(timer <= 0.0f) isSwinging = false;
    }
}

void Sword::render(SDL_Renderer* renderer){
    if(!isSwinging) return;

    SDL_SetRenderDrawColor(renderer, 255, 230, 0, 255);

    SDL_FRect rect = {
        hitboxPos.x - slashSize / 2.0f,
        hitboxPos.y - slashSize / 2.0f,
        slashSize,
        slashSize
    };
        SDL_RenderFillRectF(renderer, &rect);

}