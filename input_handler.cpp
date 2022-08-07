#include "input_handler.hpp"
#include <iostream>
#include <iterator>

// A key has a previous and current press state, to differentiate between key press, hold and release.
// Sets default key state to up/neutral.
Key::Key(SDL_Keycode keycode)
{
	this->keycode = keycode;
    state_press_prev = false;
    state_press_curr = false;
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
    up = new Key(SDLK_UP);
    down = new Key(SDLK_DOWN);
    left = new Key(SDLK_LEFT);
    right = new Key(SDLK_RIGHT);
    escape = new Key(SDLK_ESCAPE);
    action = new Key(SDLK_RETURN);
    quit = new Key(SDLK_q);
    replay = new Key(SDLK_r);
}

InputHandler::~InputHandler()
{
	delete up;
	delete down;
	delete left;
	delete right;
	delete escape;
	delete quit;
	delete action;
	delete replay;
}

// Update game keys prev and current press states.
void InputHandler::update_keys(SDL_Event *evt)
{
    SDL_PollEvent(evt);

    bool press = (evt->type == SDL_KEYDOWN);
    SDL_Keycode curr_keycode = evt->key.keysym.sym;

	Key *keycodes[] = {up, down, left, right, escape, action, quit, replay};

	for(auto key : keycodes)
	{
		key->state_press_prev = key->state_press_curr;
		if(evt->type == SDL_KEYDOWN || evt->type == SDL_KEYUP)
		{	
			if(key->keycode == curr_keycode)
			{
				key->state_press_curr = press;
			}
		}
	}
}
