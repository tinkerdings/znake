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

    wnd = new Window("Snake", tilesize*width_n_tiles, tilesize*height_n_tiles);
    rdr = new Renderer(wnd, 200, 200);
    input = new InputHandler();

    state = STATE_SPLASH;

    quit = false;
}

Game::~Game()
{
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

    rdr->clear(32, 32, 32);

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

    state = STATE_PLAY;

    rdr->clear(0, 0, 255);
    rdr->swap_buf();
}

void Game::state_play()
{
    if(input->escape.released())
    {
	state = STATE_PAUSE;
    }

    rdr->clear(32, 32, 32);

    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score++).c_str(),
		     wnd->get_width()/2, 4*tilesize,
		     2*((uint16_t)log10(score)+1)*tilesize, 2*tilesize,
		     64, 32, 255);

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

    rdr->clear(32, 32, 32);

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
    
    rdr->swap_buf();
}

void Game::state_game_over()
{

    rdr->clear(255, 0, 0);
    rdr->swap_buf();
}
