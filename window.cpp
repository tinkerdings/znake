#include "window.h"
#include <iostream>
#include <stdlib.h>

Window::Window(const char *title, int width, int height)
{
    wnd = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(!wnd)
    {
        std::cout << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
        exit(1);
    }

    /* input = new InputHandler(this); */
}

Window::~Window()
{

}
