#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "snake.hpp"
#include "window.hpp"
#include "pickup.hpp"

enum FontType
{
    FONT_TITLE, FONT_NORMAL
};
enum FontStyle
{
    STYLE_NORMAL, STYLE_3D_RG, STYLE_3D_RB, STYLE_3D_GB
};

class Renderer
{
public:
    Renderer(Window *wnd, SDL_Rect level_frame, int font_size_title, int font_size_normal);
    ~Renderer();
    void clear(Uint8 r, Uint8 g, Uint8 b);
    void swap_buf();
    void render_game_border();
    void render_snake(Snake *snake, int tilesize, int width_n_tiles);
    void render_pickup(int pos_cell_x, int pos_cell_y, int tilesize);
    void render_tiles_debug(Tile *tiles, int tilesize, int width_n_tiles, int height_n_tiles);
    void render_text(
		     FontType font_type, FontStyle font_style,
		     const char *str,
		     int x, int y, int w, int h,
		     Uint8 r, Uint8 g, Uint8 b);
private:
    SDL_Renderer *sdl_p;
    TTF_Font *font_title;
    TTF_Font *font_normal;
    SDL_Rect game_border;
};

#endif // RENDERER_HPP
