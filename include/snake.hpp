#ifndef SNAKE_HPP
#define SNAKE_HPP

#include <cstdint>
#include <vector>
#include "input_handler.hpp"
#include "tile.hpp"

enum Direction
{
    UP, DOWN, LEFT, RIGHT
};

struct SnakeSegment
{
    int pos_cell_x;
    int pos_cell_y;
};

class Snake
{
public:
    Snake(Direction direction,
	  Tile* tiles, int width_n_tiles, int height_n_tiles,
	  int tile_x, int tile_y);
    ~Snake(){};
    Tile check_next_collision();
    void handle_input(InputHandler *input);
    void update();
    void add_segment();
    std::vector<SnakeSegment> segments;
    Direction direction;
    Direction new_direction;
    bool potential_death;
    double delay;
private:
    Tile *tiles;
    int width_n_tiles;
    int height_n_tiles;
};

#endif // SNAKE_HPP
