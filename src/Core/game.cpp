#include <iostream>
#include "game.h"
#include "Player/player.h"

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr){}

Game::~Game(){}

bool Game::init(const char* title, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "Error with init" << "\n" << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );
    if(!window){
        std::cerr << "Error with window" << "\n" << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(!renderer){
        std::cerr << "Renderer error" << "\n" << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        return false;
    }

    player = std::make_unique<Player>(400, 300, 40, 40);

    isRunning = true;
    return true;
}

void Game::handleEvents(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            isRunning = false;
        }
    }
}

void Game::update(){
    if(player){
        player->update();
    }
}

void Game::render(){
    SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
    SDL_RenderClear(renderer);

    //Player
    if(player){
        player->render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean(){
    if(renderer){
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window){
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
    isRunning = false;
}