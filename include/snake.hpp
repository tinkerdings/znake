#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstdint>
#include <vector>
#include "input_handler.hpp"

enum Direction
{
    UP, DOWN, LEFT, RIGHT
};

struct SnakeSegment
{
    int32_t pos_cell_x;
    int32_t pos_cell_y;
};

class Snake
{
public:
    Snake(Direction direction, uint16_t tile_x, uint16_t tile_y);
    ~Snake(){};
    void handle_input(InputHandler *input);
    void update();
    std::vector<SnakeSegment> segments;
    Direction direction;
};

#endif // SNAKE_HPP
