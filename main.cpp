#include <iostream>
#include <SDL2/SDL.h>
#include "game.h"

int main()
{
    Game *game = new Game();
    game->run();

    game->end();

    delete game;
    return 0;
}
