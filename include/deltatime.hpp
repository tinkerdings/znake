#ifndef DELTATIME_H
#define DELTATIME_H

#include <cstdint>

extern double deltatime;
void set_deltatime();

class Timer
{
 public:
  void start_timer();
  void update_timer();
  void time_diff();
  void reset_timer();
 private:
  uint64_t prev;
  uint64_t curr;
};

#endif // DELTATIME_H
