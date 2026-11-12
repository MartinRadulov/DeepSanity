#include "bow.h"
#include "constants.h"

void Bow::attack(Vector2 playerPos, Vector2 arrowVel, float fireRate, float range){
    if(!canAttack()) return;

    Arrow arrow;
    arrow.position = playerPos;
    arrow.velocity = arrowVel;
    arrow.maxRange = range;
    arrow.disTravel = 0.0f;
    arrow.isDead = false;

    arrows.push_back(arrow);
    cooldownTimer = fireRate;
}

void Bow::movmentArrows(float deltaTime){
    for(auto& ar : arrows){
        Vector2 step = ar.velocity * deltaTime;
        ar.position += step;
        ar.disTravel += step.length();

        if(ar.disTravel >= ar.maxRange){
            ar.isDead = true;
        }
    }
}

void Bow::cleanArrows(){
    int writeIndex = 0;
    for(int readIndex = 0; readIndex < (int)arrows.size(); readIndex++){
        if(!arrows[readIndex].isDead){
            arrows[writeIndex] = arrows[readIndex];
            writeIndex++;
        }
    }
    arrows.resize(writeIndex);
}

void Bow::update(float deltaTime){
    Weapon::update(deltaTime);
    movmentArrows(deltaTime);
    cleanArrows();
}

void Bow::render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 230, 0, 255);

    for(const auto& ar : arrows){
        SDL_FRect rect = {
            ar.position.x - Constants::ARROW_HALF_SIZE,
            ar.position.y - Constants::ARROW_HALF_SIZE,
            Constants::ARROW_SIZE,
            Constants::ARROW_SIZE
        };
        SDL_RenderFillRectF(renderer, &rect);
    }
}