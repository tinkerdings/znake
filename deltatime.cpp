#include <SDL2/SDL.h>
#include "deltatime.hpp"

void set_deltatime()
{
    prev = curr;
    curr = SDL_GetPerformanceCounter();

    deltatime = (double)((curr - prev)*1000 / (double)SDL_GetPerformanceFrequency());
}

void Timer::start_timer()
{
  this->prev = SDL_GetPerformanceCounter();
}

void Timer::update_timer()
{
  this->curr = SDL_GetPerformanceCounter();
}

void Timer::time_diff()
{
  this->update_timer();
  return ((double)((this->curr - this->prev)*1000 / (double)SDL_GetPerformanceFrequency()));
}

void Timer::reset_timer()
{
  this->prev = this->curr;
}
