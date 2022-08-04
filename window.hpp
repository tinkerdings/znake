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
    uint32_t get_width();
    uint32_t get_height();

private:
    uint32_t width;
    uint32_t height;
};

#endif // WINDOW_H
