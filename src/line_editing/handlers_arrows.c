#include "../../include/line_editing.h"

void	handle_up(int key)
{
	if (key == K_UP)
	{
		clear_buffer(0);
	}
}

void	handle_left(int key)
{
	if (key == K_LEFT && g_term->iterator > 0
		&& g_term->buffer[g_term->iterator - 1] != '\n')
	{
		caret_move(1, D_LEFT);
		while (g_term->iterator > 0 &&
				((uint8_t)g_term->buffer[g_term->iterator--]) > 0x7F &&
				(g_term->buffer[g_term->iterator] & 0xC0) ==
						(g_term->buffer[g_term->iterator--] & 0x80))
			;
	}
}

void	handle_right(int key)
{
	if (key == K_RIGHT && g_term->iterator < (int)ft_strlen(g_term->buffer)
		&& g_term->iterator < MAX_INPUT)
	{
		caret_move(1, D_RIGHT);
		while (g_term->iterator < (int)ft_strlen(g_term->buffer) &&
				(g_term->buffer[g_term->iterator] & 0xC0) ==
						(g_term->buffer[g_term->iterator++] & 0x80))
			;
	}
}
