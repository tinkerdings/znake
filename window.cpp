#include "window.hpp"
#include <iostream>
#include <stdlib.h>

Window::Window(const char *title, int width, int height)
{
    sdl_p = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if(!sdl_p)
    {
        std::cout << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
        exit(1);
    }

    this->width = width;
    this->height = height;
}

Window::~Window()
{
    SDL_DestroyWindow(sdl_p);
}

uint32_t Window::get_width()
{
    return width;
}
uint32_t Window::get_height()
{
    return height;
}
