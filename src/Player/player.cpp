#include <iostream>
#include "player.h"
#include "constants.h"

Player::Player(int startX, int startY) :
    position(startX, startY){
        width = Constants::PLAYER_DEFAULT_WIDTH;
        height = Constants::PLAYER_DEFAULT_HEIGHT;
        speed = Constants::PLAYER_DEFAULT_SPEED;
        shotSpeed = 450.0f; // Pixels per second
        rangeB = 350.0f;     // Total travel distance in pixels
        rangeS = 40.0f;
        fireRate = 0.35f;   // Seconds between shots
        slashRate = 0.20f;

        // Create the bow and set as current weapon
        bow = std::make_unique<Bow>();
        sword = std::make_unique<Sword>();
        currWeapon = sword.get();
    }

Player::~Player(){}

void Player::update(float deltaTime){
    handleMovment(deltaTime);

    Vector2 center(position.x + width / 2.0f, position.y + height / 2.0f);
    if(bow) bow->update(deltaTime, center);
    if(sword) sword->update(deltaTime, center);

    handleAttack();
}

void Player::handleMovment(float deltaTime){
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    moveDir = Vector2(0.0f, 0.0f);

    if(keystates[SDL_SCANCODE_W]) moveDir.y -= 1.0f;
    if(keystates[SDL_SCANCODE_S]) moveDir.y += 1.0f;
    if(keystates[SDL_SCANCODE_A]) moveDir.x -= 1.0f;
    if(keystates[SDL_SCANCODE_D]) moveDir.x += 1.0f;

    position += moveDir.normalized() * speed * deltaTime;
}

void Player::handleAttack(){
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    Vector2 dir(0.0f, 0.0f);
    float momentum = 0.25f;

    if(keystates[SDL_SCANCODE_UP])    dir.y -= 1.0f;
    if(keystates[SDL_SCANCODE_DOWN])  dir.y += 1.0f;
    if(keystates[SDL_SCANCODE_LEFT])  dir.x -= 1.0f;
    if(keystates[SDL_SCANCODE_RIGHT]) dir.x += 1.0f;

    Vector2 arrowVel = (dir.normalized() * shotSpeed) + (moveDir * momentum * speed);

    if(dir.x != 0.0f || dir.y != 0.0f){
        Vector2 center(position.x + width / 2.0f, position.y + height / 2.0f);
        Vector2 spawnPos = center +  (dir * Constants::ATTACK_OFFSET);
        if(currWeapon == bow.get()) bow->attack(spawnPos, arrowVel, fireRate, rangeB); //BOW
        if(currWeapon == sword.get()) sword->attack(spawnPos, slashRate, rangeS, dir); //SWORD
    }
}

void Player::render(SDL_Renderer* renderer){
    SDL_FRect rect = {position.x, position.y, static_cast<float>(width), static_cast<float>(height)};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRectF(renderer, &rect);

    if(bow) bow->render(renderer);
    if(sword) sword->render(renderer);
}

void Player::switchWeapon(){
    if(currWeapon == bow.get()) currWeapon = sword.get();
    else currWeapon = bow.get();
}