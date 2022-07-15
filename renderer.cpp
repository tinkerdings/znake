#include <iostream>
#include <cstdlib>
#include "renderer.hpp"

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
    SDL_SetRenderDrawColor(sdl_p, 255, 255, 255, 255);
    SDL_RenderDrawRect(sdl_p, &game_border);
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
	Renderer::render_text(
			      font_type, STYLE_NORMAL, str,
			      x - 2, y - 2, w+8, h+8,
			      color_3d_a.r, color_3d_a.g, color_3d_a.b);
	Renderer::render_text(
			      font_type, STYLE_NORMAL, str,
			      x + 2, y + 2, w+8, h+8,
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

    SDL_Rect rect =
	{
	    .x = (int32_t)(x - w/2),
	    .y = (int32_t)(y - h/2),
	    .w = (int32_t)w,
	    .h = (int32_t)h
	};

    SDL_RenderCopy(sdl_p, text, NULL, &rect);

    SDL_DestroyTexture(text);
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
