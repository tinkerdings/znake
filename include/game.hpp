#ifndef GAME_H
#define GAME_H
#include <SDL2/SDL.h>
#include <cstdint>
#include "deltatime.hpp"
#include "snake.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "tile.hpp"

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
    Tile *tiles;
    Renderer *rdr;
    SDL_Event evt;
    Window *wnd;
    InputHandler *input;
    uint8_t tilesize;
    uint16_t width_n_tiles;
    uint16_t height_n_tiles;
    State state;
    bool quit;
    uint16_t score;
    Snake *snake;
    Timer timer_snake;
};

#endif // GAME_H
