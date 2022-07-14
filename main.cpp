#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"

int main()
{
    Game *game = new Game(16, 48, 48);

    game->run();

    delete game;
    return 0;
}
