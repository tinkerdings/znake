#include <iostream>
#include <SDL2/SDL.h>

int main()
{
    if(!SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "SDL_Init: " << SDL_GetError() << std::endl;

        return -1;
    }



    return 0;
}
