#include "../../include/line_editing.h"

void	handle_up(int key)
{
	if (key == K_UP)
	{
		cursor_move(1, D_UP);
	}
}

void	handle_left(int key)
{
	if (key == K_LEFT)
	{
		if (g_term->iterator > 0)
		{
			cursor_move(1, D_LEFT);
			g_term->iterator--;
		}
	}
}

void	handle_right(int key)
{
	if (key == K_RIGHT)
	{
		if (g_term->iterator < ft_strlen(g_term->line_buffer))
		{
			cursor_move(1, D_RIGHT);
			g_term->iterator++;
		}
	}
}

void	handle_backspace(int key)
{
	if (key == K_BSP)
	{
		g_term->line_buffer[g_term->iterator - 1] = '\0';
		handle_left(K_LEFT);
	}
}
