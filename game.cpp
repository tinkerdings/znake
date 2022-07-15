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
		     FONT_NORMAL, STYLE_3D_RG, "by Markus",
		     wnd->get_width()/2, wnd->get_height()/4 + 8*tilesize,
		     16*tilesize, 1*tilesize,
		     255, 255, 255);

    rdr->render_text(
		     FONT_TITLE, STYLE_3D_RB, "SnAkE",
		     wnd->get_width()/2, wnd->get_height()/4,
		     30*tilesize, 12*tilesize,
		     64, 255, 32);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, "[WASD] or [ARROW-KEYS] to move",
		     wnd->get_width()/2, wnd->get_height() - 24*tilesize,
		     24*tilesize, 2*tilesize,
		     32, 255, 255);
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RB, "[ESC] in-game to pause",
		     wnd->get_width()/2, wnd->get_height() - 22*tilesize,
		     16*tilesize, 1*tilesize,
		     32, 255, 255);
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RB, "[ENTER] to play",
		     wnd->get_width()/2, wnd->get_height() - 16*tilesize,
		     32*tilesize, 4*tilesize,
		     32, 255, 64);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_GB, "[ESC] or [Q] to quit",
		     wnd->get_width()/2, wnd->get_height() - 8*tilesize,
		     16*tilesize, 2*tilesize,
		     255, 255, 64);

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
    memset(tiles, 0, width_n_tiles*height_n_tiles*sizeof(Tile));
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

    if(timer_snake.diff() > snake->delay)
    {
	uint32_t snake_head_index = snake->segments[0].pos_cell_y*width_n_tiles+snake->segments[0].pos_cell_x;
	uint32_t snake_tail_index = snake->segments[snake->segments.size()-1].pos_cell_y*width_n_tiles+snake->segments[snake->segments.size()-1].pos_cell_x;

	Tile snake_collision = snake->check_next_collision();

	switch(snake_collision)
	{
	    case(EMPTY):
	    {
		snake->potential_death = false;
		snake->update();
		tiles[snake_head_index] = SNAKE;
		tiles[snake_tail_index] = EMPTY;
		break;
	    }
	    case(OUT_OF_BOUNDS):
	    case(SNAKE):
	    {
		if(snake->potential_death)
		{
		    state = STATE_GAME_OVER;
		    break;
		}
		snake->potential_death = true;

		break;
	    }
	    case(PICKUP):
	    {
		snake->potential_death = false;
		score++;

		position_pickup();

		tiles[snake_head_index] = SNAKE;
		snake->add_segment();
		snake->update();
		break;
	    }
	}

	timer_snake.reset();
    }

    rdr->clear(0, 0, 0);

    rdr->render_pickup(pickup_pos_cell_x, pickup_pos_cell_y, tilesize);
    rdr->render_snake(snake, tilesize, width_n_tiles);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score).c_str(),
		     wnd->get_width()/2, 4*tilesize,
		     2*((uint16_t)log10(score)+1)*tilesize, 2*tilesize,
		     64, 64, 255);

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

    rdr->render_pickup(pickup_pos_cell_x, pickup_pos_cell_y, tilesize);
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
		     255, 255, 64);
    
    rdr->render_game_border();

    rdr->swap_buf();
}

void Game::state_game_over()
{
    if(input->replay.released())
    {
	state = STATE_RESET_GAME;
    }
    if(input->escape.released() || input->quit.released())
    {
	state = STATE_SPLASH;
    }

    rdr->clear(0, 0, 0);
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RB, "HIGHSCORE:",
		     wnd->get_width()/2, 4*tilesize,
		     16*tilesize, 4*tilesize,
		     32, 255, 64);
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score).c_str(),
		     wnd->get_width()/2, 12*tilesize,
		     8*((uint16_t)log10(score)+1)*tilesize, 8*tilesize,
		     64, 32, 255);
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
		     FONT_NORMAL, STYLE_3D_GB, "[ESC] or [Q] to quit",
		     wnd->get_width()/2, wnd->get_height() - 4*tilesize,
		     12*tilesize, 2*tilesize,
		     255, 255, 64);
    rdr->swap_buf();
}

void Game::position_pickup()
{
    tiles[pickup_index] = EMPTY;
    do
    {
	pickup_pos_cell_x = (uint32_t)rand_range(0, width_n_tiles);
	pickup_pos_cell_y = (uint32_t)rand_range(0, height_n_tiles);
	pickup_index = (pickup_pos_cell_y*width_n_tiles) + pickup_pos_cell_x;
    }
    while(tiles[pickup_index] != EMPTY);

    tiles[pickup_index] = PICKUP;
}
