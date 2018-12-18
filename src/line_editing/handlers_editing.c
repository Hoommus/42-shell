#include "line_editing.h"
#include "shell_script.h"

void	handle_backspace(union u_char key)
{
	union u_char	k;

	if (key.lng == K_BSP && g_term->buffer->iterator > 0)
	{
		k.lng = K_LEFT;
		handle_left(k);
		toggle_state(buff_char_at(g_term->buffer->iterator));
		buff_del_symbol(g_term->buffer->iterator);
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		buffer_redraw();
	}
}

void	handle_del(union u_char key)
{
	if (key.lng == K_DEL)
	{
		if (g_term->buffer->iterator <= g_term->buffer->size)
			tputs(tgetstr("dc", NULL), 1, &ft_putc);
		if (g_term->buffer->iterator < g_term->buffer->size)
		{
			buff_del_symbol(g_term->buffer->iterator);
			buffer_redraw();
		}
	}
}

void	handle_line_kill(union u_char key)
{
	if (key.lng == CKILL && g_term->buffer->size > 0)
	{
		caret_move(g_term->buffer->size, D_LEFT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		buff_clear(0);
	}
}
