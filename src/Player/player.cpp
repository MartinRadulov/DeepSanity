#include <iostream>
#include "player.h"

Player::Player(int x, int y, int width, int height) :
    x(x), y(y), width(width), height(height){}

Player::~Player(){}

void Player::update(){}

void Player::render(SDL_Renderer* renderer){
    SDL_Rect rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}