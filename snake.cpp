#include "snake.hpp"


Snake::Snake(Direction direction,
	     Tile *tiles, uint16_t width_n_tiles, uint16_t height_n_tiles,
	     uint16_t tile_x, uint16_t tile_y)
{
    this->tiles = tiles;
    this->width_n_tiles = width_n_tiles;
    this->height_n_tiles = height_n_tiles;

    potential_death = false;

    delay = 120;
    
    SnakeSegment head = SnakeSegment();
    SnakeSegment body = SnakeSegment();

    this->direction = direction;
    this->new_direction = direction;

    head.pos_cell_x = tile_x;
    head.pos_cell_y = tile_y;

    switch(direction)
    {
	case(UP):
	{
	    body.pos_cell_x = head.pos_cell_x;
	    body.pos_cell_y = head.pos_cell_y + 1;
	    break;
	}
	case(DOWN):
	{
	    body.pos_cell_x = head.pos_cell_x;
	    body.pos_cell_y = head.pos_cell_y - 1;
	    break;
	}
	case(LEFT):
	{
	    body.pos_cell_x = head.pos_cell_x + 1;
	    body.pos_cell_y = head.pos_cell_y;
	    break;
	}
	case(RIGHT):
	{
	    body.pos_cell_x = head.pos_cell_x - 1;
	    body.pos_cell_y = head.pos_cell_y;
	    break;
	}
    }

    segments.push_back(head);
    segments.push_back(body);
}

void Snake::handle_input(InputHandler *input)
{
    if(input->up.hold())
    {
	if(direction != DOWN)
	{
	    new_direction = UP;
	}
    }
    if(input->down.hold())
    {
	if(direction != UP)
	{
	    new_direction = DOWN;
	}
    }
    if(input->left.hold())
    {
	if(direction != RIGHT)
	{
	    new_direction = LEFT;
	}
    }
    if(input->right.hold())
    {
	if(direction != LEFT)
	{
	    new_direction = RIGHT;
	}
    }
}

void Snake::add_segment()
{
    SnakeSegment segment = SnakeSegment();
    SnakeSegment end = segments[segments.size()-1];
    segment.pos_cell_x = end.pos_cell_x;
    segment.pos_cell_y = end.pos_cell_y;
    segments.push_back(segment);
    delay *= 0.98;
}

Tile Snake::check_next_collision()
{
    int32_t plus_x;
    int32_t plus_y;

    switch(new_direction)
    {
	case(UP):
	{
	    plus_x = 0;
	    plus_y = -1;
	    break;
	}
	case(DOWN):
	{
	    plus_x = 0;
	    plus_y = 1;
	    break;
	}
	case(LEFT):
	{
	    plus_x = -1;
	    plus_y = 0;
	    break;
	}
	case(RIGHT):
	{
	    plus_x = 1;
	    plus_y = 0;
	    break;
	}
    }

    int32_t tiles_index_x = segments[0].pos_cell_x + plus_x;
    int32_t tiles_index_y = segments[0].pos_cell_y + plus_y;
    int32_t tiles_index =
	(tiles_index_y * width_n_tiles) + tiles_index_x;

    if((tiles_index_x < 0) || (tiles_index_x >= width_n_tiles) ||
       (tiles_index_y < 0) || (tiles_index_y >= height_n_tiles))
    {
	return OUT_OF_BOUNDS;
    }

    return tiles[tiles_index];
}

void Snake::update()
{
    auto itr = segments.end();

    for(; itr != segments.begin(); itr--)
    {
	itr->pos_cell_x = (itr-1)->pos_cell_x;
	itr->pos_cell_y = (itr-1)->pos_cell_y;
    }

    switch(new_direction)
    {
	case(UP):
	{
	    segments[0].pos_cell_y--;
	    break;
	}
	case(DOWN):
	{
	    segments[0].pos_cell_y++;
	    break;
	}
	case(LEFT):
	{
	    segments[0].pos_cell_x--;
	    break;
	}
	case(RIGHT):
	{
	    segments[0].pos_cell_x++;
	    break;
	}
    }
    direction = new_direction;
}
