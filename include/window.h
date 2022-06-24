#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "input_handler.h"

class Window
{
public:
    Window(const char *title, int width, int height);
    ~Window();

private:
    SDL_Window *wnd = nullptr;
    InputHandler *input = nullptr;
};

#endif // WINDOW_H
