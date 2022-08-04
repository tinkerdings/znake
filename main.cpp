#include <iostream>
#include <SDL2/SDL.h>
#include "game.hpp"

int main()
{
    // Creates window and sets up game variables & states.
    // parameters specify tilesize and number of tiles in the x and y direction.
    Game *game = new Game(16, 48, 48);

    game->run();

    delete game;
    return 0;
}
