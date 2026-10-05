#include <iostream>
#include "player.h"

Player::Player(int startX, int startY) :
    position(startX, startY){
        width = 40;
        height = 40;
        speed = 5.0f;
    }

Player::~Player(){}

void Player::update(){
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    Vector2 dir(0.0f, 0.0f);

    if(keystates[SDL_SCANCODE_W]) dir.y -= 1.0f;
    if(keystates[SDL_SCANCODE_S]) dir.y += 1.0f;
    if(keystates[SDL_SCANCODE_A]) dir.x -= 1.0f;
    if(keystates[SDL_SCANCODE_D]) dir.x += 1.0f;

    position += dir.normalized() * speed;
}

void Player::render(SDL_Renderer* renderer){
    SDL_FRect rect = {position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRectF(renderer, &rect);
}