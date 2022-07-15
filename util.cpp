#include "util.hpp"
#include <cstdlib>

int32_t rand_range(int32_t low, int32_t high)
{
    return (random() % (high - low)) + low;
}
