#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "window.hpp"

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
    Renderer(Window *wnd, uint8_t font_size_title, uint8_t font_size_normal);
    ~Renderer();
    void clear(uint8_t r, uint8_t g, uint8_t b);
    void swap_buf();
    void render_text(
		     FontType font_type, FontStyle font_style,
		     const char *str,
		     uint32_t x, uint32_t y, uint32_t w, uint32_t h,
		     uint8_t r, uint8_t g, uint8_t b);
private:
    SDL_Renderer *sdl_p;
    TTF_Font *font_title;
    TTF_Font *font_normal;
};

#endif // RENDERER_HPP
