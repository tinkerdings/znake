#include "util.hpp"
#include <cstdlib>

// returns random number between low and high
int rand_range(int low, int high)
{
    return (random() % (high - low)) + low;
}
