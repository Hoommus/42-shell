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
	if (key == K_LEFT && g_term->iterator > 0)
	{
		cursor_move(1, D_LEFT);
		while (g_term->iterator > 0 &&
				((uint8_t)g_term->buffer[g_term->iterator--]) > 0x7F &&
				(g_term->buffer[g_term->iterator] & 0xC0) ==
				(g_term->buffer[g_term->iterator--] & 0x80))
			;
	}
}

void	handle_right(int key)
{
	if (key == K_RIGHT)
	{
		if (g_term->iterator < (int)ft_strlen(g_term->buffer)
				&& g_term->iterator < MAX_INPUT)
		{
			cursor_move(1, D_RIGHT);
			while (g_term->iterator < (int)ft_strlen(g_term->buffer) &&
					(g_term->buffer[g_term->iterator] & 0xC0) ==
					(g_term->buffer[g_term->iterator++] & 0x80))
				;
		}
	}
}

void	handle_backspace(int key)
{
	if (key == K_BSP)
	{
		if (g_term->iterator > 0)
		{
			handle_left(K_LEFT);
			g_term->iterator -= delete_char_at(g_term->buffer, g_term->iterator) - 1;
			tputs(tgetstr("dc", NULL), 1, &ft_putc);
		}
	}
}

void	handle_del(int key)
{
	if (key == K_DEL)
	{
		if (ft_strlen(g_term->buffer + g_term->iterator) > 0)
		{
			delete_char_at(g_term->buffer, g_term->iterator + 1);
			tputs(tgetstr("dc", NULL), 1, &ft_putc);
		}
	}
}

void	handle_line_kill(int key)
{
	if (key == CKILL)
	{
		cursor_move(g_term->iterator - 1, D_LEFT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		clear_buffer();
	}
}
