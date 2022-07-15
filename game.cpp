#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include "util.hpp"
#include "game.hpp"

Game::Game(uint8_t tilesz, uint16_t w_n_tiles, uint16_t h_n_tiles) :
    tilesize{tilesz}, width_n_tiles{w_n_tiles}, height_n_tiles{h_n_tiles}
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;

        exit(1);
    }

    srand(time(NULL));

    wnd = new Window("Snake", tilesize*(width_n_tiles + 2), tilesize*(height_n_tiles + 8));
    SDL_Rect game_border = {.x = tilesize, .y = tilesize * 7, .w = width_n_tiles * tilesize, .h = height_n_tiles * tilesize};
    rdr = new Renderer(wnd, game_border, 200, 200);
    input = new InputHandler();

    tiles = (Tile*)calloc(width_n_tiles*height_n_tiles, sizeof(uint32_t));
    if(tiles == NULL)
    {
	std::cout << "Failed to allocate tiles array" << std::endl;
	exit(1);
    }

    snake_delay = 150;

    score = 0;

    state = STATE_SPLASH;

    quit = false;
}

Game::~Game()
{
    free(tiles);
    delete snake;
    delete input;
    delete rdr;
    delete wnd;
}

void Game::run()
{
    while(!quit)
    {
	input->update_keys(&evt);

	switch(state)
	{
	    case(STATE_SPLASH):
	    {
		state_splash();
		break;
	    }
	    case(STATE_RESET_GAME):
	    {
		state_reset_game();
		break;
	    }
	    case(STATE_PLAY):
	    {
		state_play();
		break;
	    }
	    case(STATE_PAUSE):
	    {
		state_pause();
		break;
	    }
	    case(STATE_GAME_OVER):
	    {
		state_game_over();
		break;
	    }
	}

    }
}

void Game::state_splash()
{
    if(input->quit.released() || input->escape.released())
    {
	quit = true;
    }
    if(input->action.released())
    {
	state = STATE_RESET_GAME;
    }

    rdr->clear(0, 0, 0);

    rdr->render_text(
		     FONT_TITLE, STYLE_3D_RB, "SnAkE",
		     wnd->get_width()/2, wnd->get_height()/4,
		     30*tilesize, 12*tilesize,
		     64, 255, 32);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RB, "[ENTER] to play",
		     wnd->get_width()/2, wnd->get_height() - 16*tilesize,
		     32*tilesize, 4*tilesize,
		     32, 255, 64);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_GB, "[ESC/Q] to quit",
		     wnd->get_width()/2, wnd->get_height() - 8*tilesize,
		     32*tilesize, 4*tilesize,
		     255, 64, 64);

    rdr->swap_buf();
}

void Game::state_reset_game()
{
    score = 0;
    if(snake != NULL)
    {
	delete snake;
    }
    snake = new Snake(UP, tiles, width_n_tiles, height_n_tiles, width_n_tiles/2, height_n_tiles/2);
    // memset(tiles, 0, width_n_tiles*height_n_tiles*sizeof(uint32_t));
    timer_snake.start();

    position_pickup();

    state = STATE_PLAY;

    rdr->clear(0, 0, 0);
    rdr->swap_buf();
}

void Game::state_play()
{
    if(input->escape.released())
    {
	state = STATE_PAUSE;
    }

    snake->handle_input(input);

    if(timer_snake.diff() > snake_delay)
    {
	timer_snake.reset();
	Tile snake_collision = snake->update();
	uint32_t snake_last_index =
	    snake->segments[snake->segments.size()-1].pos_cell_y*width_n_tiles +
	    snake->segments[snake->segments.size()-1].pos_cell_x;
	switch(snake_collision)
	{
	    case(OUT_OF_BOUNDS):
	    case(SNAKE):
	    {
		state = STATE_GAME_OVER;
		break;
	    }
	    case(PICKUP):
	    {
		snake_delay *= 0.98;
		position_pickup();
		score++;
		break;
	    }
	}
	uint32_t tiles_index = snake->segments[1].pos_cell_y*width_n_tiles+snake->segments[1].pos_cell_x;
	tiles[tiles_index] = SNAKE;
	tiles[snake_last_index] = EMPTY;
    }

    rdr->clear(0, 0, 0);

    rdr->render_pickup(pickup_pos_cell_x, pickup_pos_cell_y, tilesize);
    rdr->render_snake(snake, tilesize, width_n_tiles);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score).c_str(),
		     wnd->get_width()/2, 4*tilesize,
		     2*((uint16_t)log10(score)+1)*tilesize, 2*tilesize,
		     64, 32, 255);

    rdr->render_game_border();

    rdr->swap_buf();
}

void Game::state_pause()
{
    if(input->escape.released() || input->action.released())
    {
	state = STATE_PLAY;
    }

    if(input->quit.released())
    {
	state = STATE_SPLASH;
    }

    rdr->clear(0, 0, 0);

    rdr->render_snake(snake, tilesize, width_n_tiles);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score).c_str(),
		     wnd->get_width()/2, 4*tilesize,
		     2*((uint16_t)log10(score)+1)*tilesize, 2*tilesize,
		     64, 32, 255);

    rdr->render_text(
		     FONT_TITLE, STYLE_3D_RB, "PaUsEd",
		     wnd->get_width()/2, wnd->get_height()/2,
		     30*tilesize, 12*tilesize,
		     255, 255, 64);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RB, "[ESC] or [ENTER] to resume",
		     wnd->get_width()/2, wnd->get_height() - 8*tilesize,
		     24*tilesize, 4*tilesize,
		     32, 255, 64);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_GB, "[Q] to quit",
		     wnd->get_width()/2, wnd->get_height() - 4*tilesize,
		     12*tilesize, 2*tilesize,
		     255, 64, 64);
    
    rdr->render_game_border();

    rdr->swap_buf();
}

void Game::state_game_over()
{
    if(input->replay.released())
    {
	state = STATE_RESET_GAME;
    }
    if(input->escape.released())
    {
	state = STATE_SPLASH;
    }

    rdr->clear(0, 0, 0);
    rdr->render_text(
		     FONT_TITLE, STYLE_3D_GB, "GaMe OvEr",
		     wnd->get_width()/2, wnd->get_height()/2,
		     40*tilesize, 8*tilesize,
		     255, 64, 64);
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RB, "[R] to play again",
		     wnd->get_width()/2, wnd->get_height() - 8*tilesize,
		     24*tilesize, 4*tilesize,
		     32, 255, 64);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_GB, "[ESC] to quit",
		     wnd->get_width()/2, wnd->get_height() - 4*tilesize,
		     12*tilesize, 2*tilesize,
		     255, 64, 64);
    rdr->swap_buf();
}

void Game::position_pickup()
{
    tiles[pickup_index] = EMPTY;
    pickup_pos_cell_x = rand_range(0, width_n_tiles);
    pickup_pos_cell_y = rand_range(0, height_n_tiles);
    pickup_index = pickup_pos_cell_y*width_n_tiles + pickup_pos_cell_x;
    tiles[pickup_index] = PICKUP;
}
