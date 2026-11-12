#pragma once
#include <SDL2/SDL.h>
#include "Math/vector.h"
#include <memory>
#include "Weapons/weapon.h"
#include "Weapons/bow.h"
#include "Weapons/sword.h"

class Player{
private:
    Vector2 position;
    int width;
    int height;
    float speed;
    float shotSpeed;
    float rangeB;
    float rangeS;
    float fireRate;
    float slashRate;
    std::unique_ptr<Bow> bow;
    std::unique_ptr<Sword> sword;
    Weapon* currWeapon;
    Vector2 moveDir;

public:
    Player(int startX, int startY);
    ~Player();

    void handleMovment(float deltaTime);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    void handleAttack();
    void switchWeapon();
};