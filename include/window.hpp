#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <SDL2/SDL.h>
#include "input_handler.hpp"

class Window
{
public:
    Window(const char *title, int width, int height);
    ~Window();
    SDL_Window *sdl_p = nullptr;
    int get_width();
    int get_height();

private:
    int width;
    int height;
};

#endif // WINDOW_H
