#include "input_handler.hpp"
#include <iostream>
#include <iterator>

// A key has a previous and current press state, to differentiate between key press, hold and release.
// Sets default key state to up/neutral.
Key::Key()
{
    state_press_prev = false;
    state_press_curr = false;
}

// Adds keycode to be checked for game key.
void Key::add_keycode(SDL_Keycode code)
{
    keycode.push_back(code);
}

// Checks key event, pressed, hold, and released.
bool Key::pressed()
{
    return (!state_press_prev && state_press_curr);
}

bool Key::hold()
{
    return (state_press_prev && state_press_curr);
}

bool Key::released()
{
    return (state_press_prev && !state_press_curr);
}

// Adds default keycodes to game keys.
InputHandler::InputHandler()
{
    up.add_keycode(SDLK_UP);
    up.add_keycode(SDLK_w);

    down.add_keycode(SDLK_DOWN);
    down.add_keycode(SDLK_s);
    
    left.add_keycode(SDLK_LEFT);
    left.add_keycode(SDLK_a);

    right.add_keycode(SDLK_RIGHT);
    right.add_keycode(SDLK_d);

    escape.add_keycode(SDLK_ESCAPE);

    action.add_keycode(SDLK_RETURN);

    quit.add_keycode(SDLK_q);

    replay.add_keycode(SDLK_r);
}

// Sets default keycodes passed in for game keys.
InputHandler::InputHandler(SDL_Keycode default_up, SDL_Keycode default_down,
			   SDL_Keycode default_left, SDL_Keycode default_right,
			   SDL_Keycode default_escape, SDL_Keycode default_action,
			   SDL_Keycode default_quit, SDL_Keycode default_replay)
{
    up.add_keycode(default_up);
    down.add_keycode(default_down);
    left.add_keycode(default_left);
    right.add_keycode(default_right);
    escape.add_keycode(default_escape);
    action.add_keycode(default_action);
    quit.add_keycode(default_quit);
    replay.add_keycode(default_replay);
}

// Update game keys prev and current press states.
void InputHandler::update_keys(SDL_Event *evt)
{
    SDL_PollEvent(evt);

    bool press = (evt->type == SDL_KEYDOWN);
    auto curr_keycode = evt->key.keysym.sym;
    // For looping over all game keys.
    Key *temp[] = {&up, &down, &left, &right, &escape, &action, &quit, &replay, nullptr};

    for(int i = 0; temp[i] != nullptr; i++)
    {
	temp[i]->state_press_prev = temp[i]->state_press_curr;
	for(auto itr = temp[i]->keycode.begin(); itr != temp[i]->keycode.end(); itr++)
	{
	    if(*itr == curr_keycode)
	    {
		temp[i]->state_press_curr = press;
	    }
	}
    }
}
