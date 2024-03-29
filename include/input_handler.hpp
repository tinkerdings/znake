#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <vector>

class Key
{
public:
    Key(SDL_Keycode keycode);
    ~Key(){};
    bool pressed();
    bool hold();
    bool released();
    bool state_press_prev;
    bool state_press_curr;
    SDL_Keycode keycode;
};

class InputHandler
{
public:
    InputHandler();
    ~InputHandler();

    void update_keys(SDL_Event *evt);

    Key *up;
    Key *down;
    Key *left;
    Key *right;
    Key *escape;
    Key *action;
    Key *quit;
    Key *replay;
};

#endif // INPUT_HANDLER_H
