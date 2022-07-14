#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <cstdint>
#include "window.hpp"
#include "renderer.hpp"

enum State
{
    STATE_SPLASH, STATE_RESET_GAME, STATE_PLAY, STATE_PAUSE, STATE_GAME_OVER
};

class Game
{
public:
    Game(uint8_t tilesz, uint16_t w_n_tiles, uint16_t h_n_tiles);
    ~Game();
    void run();
    void state_splash();
    void state_reset_game();
    void state_play();
    void state_pause();
    void state_game_over();

private:
    Renderer *rdr;
    SDL_Event evt;
    Window *wnd;
    InputHandler *input;
    uint8_t tilesize;
    uint16_t width_n_tiles;
    uint16_t height_n_tiles;
    State state;
    bool quit;
};

#endif // GAME_H
