#include "line_editing.h"

void	handle_up(int key)
{
	if (key == K_UP)
	{
		reset_buffer(0);
	}
}

void	handle_down(int key)
{
	if (key == K_DOWN)
	{
		
	}
}

void	handle_left(int key)
{
	if (key == K_LEFT && g_term->v_buffer->iterator > 0
		&& ft_strcmp(buff_char_at(g_term->v_buffer->iterator - 1), "\n") != 0)
	{
		caret_move(1, D_LEFT);
		g_term->v_buffer->iterator--;
	}
}

void	handle_right(int key)
{
	if (key == K_RIGHT && g_term->v_buffer->iterator < g_term->v_buffer->size
		&& ft_strcmp(buff_char_at(g_term->v_buffer->iterator), "\0") != 0)
	{
		caret_move(1, D_RIGHT);
		g_term->v_buffer->iterator++;
	}
}
