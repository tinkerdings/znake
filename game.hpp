#ifndef GAME_HPP
#define GAME_HPP
#include <SDL2/SDL.h>
#include <cstdint>
#include "deltatime.hpp"
#include "snake.hpp"
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
    void position_pickup();

private:
    Tile *tiles;
    Renderer *rdr;
    SDL_Event evt;
    Window *wnd;
    InputHandler *input;
    uint8_t tilesize;
    uint32_t width_n_tiles;
    uint32_t height_n_tiles;
    State state;
    bool quit;
    uint16_t score;
    Snake *snake;
    Timer timer_snake;
    volatile uint32_t pickup_pos_cell_x;
    volatile uint32_t pickup_pos_cell_y;
    volatile uint32_t pickup_index;
};

#endif // GAME_H
