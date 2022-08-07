#include "snake.hpp"

// Creates snake
Snake::Snake(Direction direction,
	     Tile *tiles, int width_n_tiles, int height_n_tiles,
	     int tile_x, int tile_y)
{
    this->tiles = tiles;
    this->width_n_tiles = width_n_tiles;
    this->height_n_tiles = height_n_tiles;

    // Might the snake collide with wall or itself next update
    potential_death = false;

    // delay in ms between snake update.
    delay = 120;
    
    // Create head and body segment.
    SnakeSegment head = SnakeSegment();
    SnakeSegment body = SnakeSegment();

    this->direction = direction;
    this->new_direction = direction;

    // Sets start position of head
    head.pos_cell_x = tile_x;
    head.pos_cell_y = tile_y;

    // Sets start position of body, depending on start direction.
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

    // Store head and body segment in segments vector.
    segments.push_back(head);
    segments.push_back(body);
}

// Handle snake user input.
void Snake::handle_input(InputHandler *input)
{
    // Change directions.
    if(input->up->hold())
    {
		if(direction != DOWN)
		{
			new_direction = UP;
		}
    }
    if(input->down->hold())
    {
		if(direction != UP)
		{
			new_direction = DOWN;
		}
    }
    if(input->left->hold())
    {
		if(direction != RIGHT)
		{
			new_direction = LEFT;
		}
    }
    if(input->right->hold())
    {
		if(direction != LEFT)
		{
			new_direction = RIGHT;
		}
    }
}

// add a new snake segment.
void Snake::add_segment()
{
    SnakeSegment segment = SnakeSegment();
    SnakeSegment end = segments[segments.size()-1];
    segment.pos_cell_x = end.pos_cell_x;
    segment.pos_cell_y = end.pos_cell_y;
    segments.push_back(segment);
    delay *= 0.98; // Decrease snake update delay, so snake moves faster depending on its number
    // of segments.
}

// Check next update collisions.
Tile Snake::check_next_collision()
{
    int plus_x;
    int plus_y;

    // sets x and y dir offsets depending on direction.
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

    // Calculates next tile index, aka where the snake will move in the gameboard tiles.
    int tiles_index_x = segments[0].pos_cell_x + plus_x;
    int tiles_index_y = segments[0].pos_cell_y + plus_y;
    int tiles_index =
	(tiles_index_y * width_n_tiles) + tiles_index_x;

    // Outside game board.
    if((tiles_index_x < 0) || (tiles_index_x >= width_n_tiles) ||
       (tiles_index_y < 0) || (tiles_index_y >= height_n_tiles))
    {
	return OUT_OF_BOUNDS;
    }

    return tiles[tiles_index];
}

// Moves snake segments. 
void Snake::update()
{
    // moves segment to the position of the one before it.
	for(int i = segments.size()-1; i > 0; i--)
	{
		segments[i].pos_cell_x = segments[i-1].pos_cell_x;
		segments[i].pos_cell_y = segments[i-1].pos_cell_y;
	}

    // Moves head segment.
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
