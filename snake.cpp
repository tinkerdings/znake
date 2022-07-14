#include "snake.hpp"

Snake::Snake(Direction direction, uint16_t tile_x, uint16_t tile_y)
{
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

void Snake::update()
{
    for(auto itr = segments.end(); itr != segments.begin(); itr--)
    {
	itr->pos_cell_x = (itr-1)->pos_cell_x;
	itr->pos_cell_y = (itr-1)->pos_cell_y;
    }
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
}
