#include "sword.h"
#include "constants.h"

void Sword::attack(Vector2 playerPos, float fireRate, float range, Vector2 dir){
    if(!canAttack()) return;

    hitboxPos = playerPos;
    slashSize = range;
    isSwinging = true;
    timer = slashDur;
    cooldownTimer = fireRate;
    slashDir = dir;
}

void Sword::update(float deltaTime, Vector2 playerPos){
    Weapon::update(deltaTime, playerPos);
    
    if(isSwinging){
        timer -= deltaTime;
        if(timer <= 0.0f) isSwinging = false;
    }
    moveSlash(playerPos);
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

void Sword::moveSlash(Vector2 playerPos){
    hitboxPos = playerPos +  (slashDir * Constants::ATTACK_OFFSET);
}