#include "util.hpp"
#include <cstdlib>

// returns random number between low and high
int32_t rand_range(int32_t low, int32_t high)
{
    return (rand() % (high - low)) + low;
}
