#include <SDL2/SDL.h>
#include "deltatime.hpp"

//void set_deltatime()
//{
//    prev = curr;
//    curr = SDL_GetPerformanceCounter();
//
//    deltatime = (double)((curr - prev)*1000 / (double)SDL_GetPerformanceFrequency());
//}

void Timer::start()
{
  this->prev = SDL_GetPerformanceCounter();
}

void Timer::update()
{
  this->curr = SDL_GetPerformanceCounter();
}

double Timer::diff()
{
  this->update();
  return ((double)((this->curr - this->prev)*1000 / (double)SDL_GetPerformanceFrequency()));
}

void Timer::reset()
{
  this->prev = this->curr;
}
