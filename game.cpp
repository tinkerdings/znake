#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include "util.hpp"
#include "game.hpp"

// Creates window, initializes tiles && sets game state to menu screen.
Game::Game(uint8_t tilesz, uint16_t w_n_tiles, uint16_t h_n_tiles) :
    tilesize{tilesz}, width_n_tiles{w_n_tiles}, height_n_tiles{h_n_tiles}
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;

        exit(1);
    }

    // sets random seed, used by rand_range function.
    srand(time(NULL));

    // Creates an SDL window.
    wnd = new Window("Snake", tilesize*(width_n_tiles + 2), tilesize*(height_n_tiles + 8));
    // game board edges
    SDL_Rect game_border = {.x = tilesize, .y = tilesize * 7, .w = width_n_tiles * tilesize, .h = height_n_tiles * tilesize};
    // contains rendering functions
    rdr = new Renderer(wnd, game_border, 200, 200);
    // handles input.
    input = new InputHandler();

    // Sets tile index of pickup object.
    pickup_index = 0;
    // allocates space for game tiles and sets them to EMPTY.
    tiles = (Tile*)calloc(width_n_tiles*height_n_tiles, sizeof(uint32_t));
    if(tiles == NULL)
    {
	std::cout << "Failed to allocate tiles array" << std::endl;
	exit(1);
    }

    // Sets beginning score to 0
    score = 0;

    // STATE_SPLASH is menu screen.
    state = STATE_SPLASH;

    // Makes sure the window does not close.
    quit = false;
}

// Deletes allocated objects.
Game::~Game()
{
    free(tiles);
    delete snake;
    delete input;
    delete rdr;
    delete wnd;
}

// Checks which gamestate is current, and runs associated state function.
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

// Menu state function
void Game::state_splash()
{
    // Quit program.
    if(input->quit.released() || input->escape.released())
    {
	quit = true;
    }
    if(input->action.released())
    {
	state = STATE_RESET_GAME;
    }

    // Clear screen
    rdr->clear(0, 0, 0);

    // Render menu text
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, "by Markus",
		     wnd->get_width()/2, wnd->get_height()/4 + 8*tilesize,
		     16*tilesize, 1*tilesize,
		     255, 255, 255);

    rdr->render_text(
		     FONT_TITLE, STYLE_3D_RB, "ZnAkE",
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

    // Swap backbuffer. to render next frame.
    rdr->swap_buf();
}

// gets runned before game starts or when restarted after game over
void Game::state_reset_game()
{
    // reset score
    score = 0;

    // Recreate snake object to reset values.
    if(snake != NULL)
    {
	delete snake;
    }

    snake = new Snake(UP, tiles, width_n_tiles, height_n_tiles, width_n_tiles/2, height_n_tiles/2);
    memset(tiles, 0, width_n_tiles*height_n_tiles*sizeof(Tile));
    // reset snake update timer.
    timer_snake.start();

    // reposition pickup at random location.
    position_pickup();

    state = STATE_PLAY;

    // Clear screen
    rdr->clear(0, 0, 0);
    rdr->swap_buf();
}

// Play state, this is the state when playing the game.
void Game::state_play()
{
    if(input->escape.released())
    {
	state = STATE_PAUSE;
    }

    // Handle snake objects relevant input.
    snake->handle_input(input);

    // update snake if timer has reached certain delay.
    if(timer_snake.diff() > snake->delay)
    {
	uint32_t snake_head_index = snake->segments[0].pos_cell_y*width_n_tiles+snake->segments[0].pos_cell_x;
	uint32_t snake_tail_index = snake->segments[snake->segments.size()-1].pos_cell_y*width_n_tiles+snake->segments[snake->segments.size()-1].pos_cell_x;

	// Handle collision with different tiles.
	Tile snake_collision = snake->check_next_collision();

	switch(snake_collision)
	{
	    // No collision move ahead.
	    case(EMPTY):
	    {
		snake->potential_death = false;
		snake->update();
		tiles[snake_head_index] = SNAKE;
		tiles[snake_tail_index] = EMPTY;
		break;
	    }
	    // might die on next step, since tile ahead is snake or out of bounds.
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
	    // increase score and add a segment to the snake, hit pickup.
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

	// Reset snake update timer.
	timer_snake.reset();
    }

    // Rendering
    // clear screen
    rdr->clear(0, 0, 0);

    // Render pickup
    rdr->render_pickup(pickup_pos_cell_x, pickup_pos_cell_y, tilesize);
    // Render snake
    rdr->render_snake(snake, tilesize, width_n_tiles);

    // Render score
    rdr->render_text(
		     FONT_NORMAL, STYLE_3D_RG, std::to_string(score).c_str(),
		     wnd->get_width()/2, 4*tilesize,
		     2*((uint16_t)log10(score)+1)*tilesize, 2*tilesize,
		     64, 64, 255);

    // Render border around game board
    rdr->render_game_border();

    // Swap backbuffer.
    rdr->swap_buf();
}

// Pause state
void Game::state_pause()
{
    // unpause
    if(input->escape.released() || input->action.released())
    {
	state = STATE_PLAY;
    }

    // Return to main menu
    if(input->quit.released())
    {
	state = STATE_SPLASH;
    }

    // Rendering
    rdr->clear(0, 0, 0);

    // Still render pickup and snake.
    rdr->render_pickup(pickup_pos_cell_x, pickup_pos_cell_y, tilesize);
    rdr->render_snake(snake, tilesize, width_n_tiles);

    // Render text
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

// Game over state
void Game::state_game_over()
{
    // Restart new game
    if(input->replay.released())
    {
	state = STATE_RESET_GAME;
    }
    // Back to menu
    if(input->escape.released() || input->quit.released())
    {
	state = STATE_SPLASH;
    }

    // Rendering
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

// Randomize pickup position
void Game::position_pickup()
{
    // as long as the pickup position is occupied, randomize again.
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
