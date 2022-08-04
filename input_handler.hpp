#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <vector>

class Key
{
public:
    Key();
    ~Key(){};
    void add_keycode(SDL_Keycode code);
    bool pressed();
    bool hold();
    bool released();
    bool state_press_prev;
    bool state_press_curr;
    std::vector<SDL_Keycode> keycode;
};

class InputHandler
{
public:
    InputHandler();
    InputHandler(SDL_Keycode default_up, SDL_Keycode default_down,
		 SDL_Keycode default_left, SDL_Keycode default_right,
		 SDL_Keycode default_escape, SDL_Keycode default_action,
		 SDL_Keycode default_quit, SDL_Keycode default_replay);
    ~InputHandler(){};

    void update_keys(SDL_Event *evt);

    Key up;
    Key down;
    Key left;
    Key right;
    Key escape;
    Key action;
    Key quit;
    Key replay;
};

#endif // INPUT_HANDLER_H
