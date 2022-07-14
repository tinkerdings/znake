#include "util.hpp"
#include <cstdlib>

uint32_t rand_range(uint32_t low, uint32_t high)
{
    return (random() % (high - low)) + low;
}
