#include "snake.hpp"

Snake::Snake(Direction direction,
	     Tile *tiles, uint16_t width_n_tiles, uint16_t height_n_tiles,
	     uint16_t tile_x, uint16_t tile_y)
{
    this->tiles = tiles;
    this->width_n_tiles = width_n_tiles;
    this->height_n_tiles = height_n_tiles;
    
    SnakeSegment head = SnakeSegment();
    SnakeSegment body = SnakeSegment();

    this->direction = direction;

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
    if(input->up.pressed())
    {
	if(direction != DOWN)
	{
	    direction = UP;
	}
    }
    if(input->down.pressed())
    {
	if(direction != UP)
	{
	    direction = DOWN;
	}
    }
    if(input->left.pressed())
    {
	if(direction != RIGHT)
	{
	    direction = LEFT;
	}
    }
    if(input->right.pressed())
    {
	if(direction != LEFT)
	{
	    direction = RIGHT;
	}
    }
}

Tile Snake::check_next_collision()
{
    int32_t plus_x;
    int32_t plus_y;

    switch(direction)
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
    int32_t tiles_index_y = segments[0].pos_cell_y+plus_y;
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
    for(auto itr = segments.end(); itr != segments.begin(); itr--)
    {
	itr->pos_cell_x = (itr-1)->pos_cell_x;
	itr->pos_cell_y = (itr-1)->pos_cell_y;
    }
    Tile collision = check_next_collision();
    switch(collision)
    {
	case(EMPTY):
	{
	    switch(direction)
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
	    break;
	}
	case(PICKUP):
	{
	    break;
	}
	case(SNAKE):
	case(OUT_OF_BOUNDS):
	{
	    break;
	}
    }
}
