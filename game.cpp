#include <SDL2/SDL.h>
#include <stdlib.h>
#include <iostream>
#include "game.h"

Game::Game()
{
    if(!SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "SDL_Init: " << SDL_GetError() << std::endl;

        exit(1);
    }
}

Game::~Game()
{

}

void Game::run()
{
    this->wnd = new Window("Snake", 800, 600);
}

void Game::end()
{
    delete this->wnd;
}
