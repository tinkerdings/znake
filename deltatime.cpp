#include <SDL2/SDL.h>
#include "deltatime.hpp"

// start timer
void Timer::start()
{
  this->prev = SDL_GetPerformanceCounter();
}

// update timer
void Timer::update()
{
  this->curr = SDL_GetPerformanceCounter();
}

// Get timer time elapsed since timer start or reset.
double Timer::diff()
{
  this->update();
  return ((double)((this->curr - this->prev)*1000 / (double)SDL_GetPerformanceFrequency()));
}

// reset timer
void Timer::reset()
{
  this->prev = this->curr;
}
