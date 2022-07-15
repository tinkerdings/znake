#include <iostream>
#include <cstdlib>
#include "renderer.hpp"
#include "util.hpp"

Renderer::Renderer(Window *wnd, SDL_Rect game_border, uint8_t font_size_title, uint8_t font_size_normal)
{
    sdl_p = SDL_CreateRenderer(wnd->sdl_p, -1, SDL_RENDERER_ACCELERATED);
    if(!sdl_p)
    {
	std::cout << "Failed to create SDL_Renderer: " << SDL_GetError() << std::endl;
	exit(1);
    }

    this->game_border = game_border;

    if(TTF_Init() < 0)
    {
	std::cout << "Failed to init SDL_TTF: " << SDL_GetError() << std::endl;
	exit(1);
    }
    font_title = TTF_OpenFont("res/font/title_font.ttf", font_size_title);
    if(font_title == NULL)
    {
	std::cout << "failed to open title font: " << SDL_GetError() << std::endl;
	exit(1);
    }
    font_normal = TTF_OpenFont("res/font/normal_font.ttf", font_size_normal);
    if(font_normal == NULL)
    {
	std::cout << "failed to open normal font: " << SDL_GetError() << std::endl;
	exit(1);
    }

}

void Renderer::render_game_border()
{
    int32_t min = -1;
    int32_t max = 1;
    int32_t x_off = rand_range(min, max);
    int32_t y_off = rand_range(min, max);
    SDL_Rect rect_a = {game_border.x + x_off, game_border.y + y_off, game_border.w, game_border.h};

    x_off = rand_range(min, max);
    y_off = rand_range(min, max);
    SDL_Rect rect_b = {game_border.x + x_off, game_border.y + y_off, game_border.w, game_border.h};

    x_off = rand_range(min, max);
    y_off = rand_range(min, max);
    SDL_Rect rect_c = {game_border.x + x_off, game_border.y + y_off, game_border.w, game_border.h};

    SDL_SetRenderDrawColor(sdl_p, 255, 64, 64, 255);
    SDL_RenderDrawRect(sdl_p, &rect_a);

    SDL_SetRenderDrawColor(sdl_p, 64, 64, 255, 255);
    SDL_RenderDrawRect(sdl_p, &rect_b);

    SDL_SetRenderDrawColor(sdl_p, 255, 255, 255, 255);
    SDL_RenderDrawRect(sdl_p, &rect_c);
}

void Renderer::render_pickup(uint32_t pos_cell_x, uint32_t pos_cell_y, uint8_t tilesize)
{
        render_text(FONT_NORMAL, STYLE_3D_RB,
    		"$",
    		tilesize/2 + game_border.x + pos_cell_x * tilesize,
    		tilesize/2 + game_border.y + pos_cell_y * tilesize,
    		tilesize, tilesize, 255, 255, 64);
}

void Renderer::render_snake(Snake *snake, uint8_t tilesize, uint16_t width_n_tiles)
{
    render_text(FONT_NORMAL, STYLE_3D_RB,
			  "@",
			  tilesize/2 + game_border.x + snake->segments[0].pos_cell_x * tilesize,
			  tilesize/2 + game_border.y + snake->segments[0].pos_cell_y * tilesize,
			  tilesize, tilesize, 64, 255, 64);
    for(auto i = 1; i < snake->segments.size(); i++)
    {
	render_text(FONT_NORMAL, STYLE_3D_RB,
			    "#",
			    tilesize/2 + game_border.x + snake->segments[i].pos_cell_x * tilesize,
			    tilesize/2 + game_border.y + snake->segments[i].pos_cell_y * tilesize,
			    tilesize, tilesize, 64, 255, 64);
    }
}

void Renderer::render_text(
			   FontType font_type, FontStyle font_style,
			   const char *str,
			   uint32_t x, uint32_t y, uint32_t w, uint32_t h,
			   uint8_t r, uint8_t g, uint8_t b)
{
    TTF_Font *font;
    SDL_Color color_text = {.r = r, .g = g, .b = b, .a = 255};
    SDL_Surface *srf_txt;
    int32_t rand_off_x;
    int32_t rand_off_y;
    int32_t half_rand_off_x;
    int32_t half_rand_off_y;

    switch(font_type)
    {
	case(FONT_TITLE):
	{
	    font = font_title;
	    break;
	}
	case(FONT_NORMAL):
	{
	    font = font_normal;
	    break;
	}
    }

    if(font_style != STYLE_NORMAL)
    {
	SDL_Color color_3d_a = {.r=64, .g=64, .b=64, .a=255};
	SDL_Color color_3d_b = {.r=64, .g=64, .b=64, .a=255};
	switch(font_style)
	{
	    case(STYLE_3D_RG):
	    {
		color_3d_a.r = 255;
		color_3d_b.g = 255;
		break;
	    }
	    case(STYLE_3D_RB):
	    {
		color_3d_a.r = 255;
		color_3d_b.b = 255;
		break;
	    }
	    case(STYLE_3D_GB):
	    {
		color_3d_a.g = 255;
		color_3d_b.b = 255;
		break;
	    }
	}

	rand_off_x = rand_range(-1, 1);
	rand_off_y = rand_range(-1, 1);
	half_rand_off_x = rand_off_x / 2;
	half_rand_off_y = rand_off_y / 2;
	Renderer::render_text(
			      font_type, STYLE_NORMAL, str,
			      x - rand_off_x, y - rand_off_y, w+half_rand_off_x, h+half_rand_off_y,
			      color_3d_a.r, color_3d_a.g, color_3d_a.b);
	Renderer::render_text(
			      font_type, STYLE_NORMAL, str,
			      x + rand_off_x, y + rand_off_y, w+half_rand_off_x, h+half_rand_off_y,
			      color_3d_b.r, color_3d_b.g, color_3d_b.b);
    }

    srf_txt = TTF_RenderText_Solid(font, str, color_text);
    if(srf_txt == NULL)
    {
	std::cout << "Failed to create SDL_Surface for text: " << SDL_GetError() << std::endl;
	exit(1);
    }

    SDL_Texture *text = SDL_CreateTextureFromSurface(sdl_p, srf_txt);
    if(text == NULL)
    {
	std::cout << "Failed to create SDL_Texture*: " << SDL_GetError() << std::endl;
	exit(1);
    }
    SDL_FreeSurface(srf_txt);

    rand_off_x = rand_range(-1, 1);
    rand_off_y = rand_range(-1, 1);

    SDL_Rect rect =
	{
	    .x = (int32_t)(x - w/2) + rand_off_x,
	    .y = (int32_t)(y - h/2) + rand_off_y,
	    .w = (int32_t)w,
	    .h = (int32_t)h
	};

    SDL_RenderCopy(sdl_p, text, NULL, &rect);

    SDL_DestroyTexture(text);
}

void Renderer::render_tiles_debug(Tile *tiles, uint8_t tilesize, uint32_t width_n_tiles, uint32_t height_n_tiles)
{
    for(int r = 0; r < height_n_tiles; r++)
    {
	for(int c = 0; c < width_n_tiles; c++)
	{
	    switch(tiles[(r*width_n_tiles) + c])
	    {
		case(EMPTY):
		{
		    SDL_SetRenderDrawColor(sdl_p, 64, 64, 64, 255);
		    break;
		}
		case(PICKUP):
		{
		    SDL_SetRenderDrawColor(sdl_p, 255, 128, 0, 255);

		    break;
		}
		case(SNAKE):
		{
		    SDL_SetRenderDrawColor(sdl_p, 0, 255, 0, 255);

		    break;
		}
		case(OUT_OF_BOUNDS):
		{
		    SDL_SetRenderDrawColor(sdl_p, 255, 0, 0, 255);

		    break;
		}
	    }
	    SDL_Rect rect = {game_border.x + c * tilesize, game_border.y + r * tilesize, tilesize, tilesize};
	    SDL_RenderFillRect(sdl_p, &rect);
	}
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(sdl_p);
    TTF_CloseFont(font_title);
    TTF_CloseFont(font_normal);
}

void Renderer::swap_buf()
{
    SDL_RenderPresent(sdl_p);
}

void Renderer::clear(uint8_t r, uint8_t g, uint8_t b)
{
    SDL_SetRenderDrawColor(sdl_p, r, g, b, 255);
    SDL_RenderClear(sdl_p);
}
