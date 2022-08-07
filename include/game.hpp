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
    Game(int tilesz, int w_n_tiles, int h_n_tiles);
    ~Game();
    void run();
    void state_splash();
    void state_reset_game();
    void state_play();
    void state_pause();
    void state_game_over();
    void position_pickup();

private:
    Tile *tiles = nullptr;
    Renderer *rdr = nullptr;
    SDL_Event evt;
    Window *wnd = nullptr;
    InputHandler *input = nullptr;
    int tilesize;
    int width_n_tiles;
    int height_n_tiles;
    State state;
    bool quit;
    int score;
    Snake *snake = nullptr;
    Timer timer_snake;
    int pickup_pos_cell_x;
    int pickup_pos_cell_y;
    int pickup_index;
};

#endif // GAME_H
