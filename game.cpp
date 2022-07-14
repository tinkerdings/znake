#include <SDL2/SDL.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>
#include "game.hpp"

Game::Game(uint8_t tilesz, uint16_t w_n_tiles, uint16_t h_n_tiles) :
    tilesize{tilesz}, width_n_tiles{w_n_tiles}, height_n_tiles{h_n_tiles}
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;

        exit(1);
    }

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
		     FONT_NORMAL, STYLE_3D_GB, "[Q] to quit",
		     wnd->get_width()/2, wnd->get_height() - 8*tilesize,
		     32*tilesize, 4*tilesize,
		     255, 64, 64);

    rdr->swap_buf();
}

void Game::state_reset_game()
{
    score = 0;
    snake = new Snake(UP, width_n_tiles/2, height_n_tiles/2);
    timer_snake.start();

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

    if(timer_snake.diff() > 100)
    {
	timer_snake.reset();
	snake->update();
    }

    rdr->clear(0, 0, 0);

    rdr->render_snake(snake, tilesize, width_n_tiles);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score++).c_str(),
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
		     FONT_TITLE, STYLE_3D_GB, "PaUsEd",
		     wnd->get_width()/2, wnd->get_height()/2,
		     30*tilesize, 12*tilesize,
		     255, 64, 64);

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

    rdr->clear(0, 0, 0);
    rdr->swap_buf();
}
