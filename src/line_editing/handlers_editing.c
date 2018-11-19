#include "line_editing.h"
#include "shell_script.h"

void	handle_backspace(u_int64_t key)
{
	if (key == K_BSP && g_term->buffer->iterator > 0)
	{
		handle_left(K_LEFT);
		toggle_state(buff_char_at(g_term->buffer->iterator));
		buff_del_symbol(g_term->buffer->iterator);
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		buffer_redraw(-1);
	}
}

void	handle_del(u_int64_t key)
{
	if (key == K_DEL && g_term->buffer->iterator < g_term->buffer->size)
	{
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		buffer_redraw(0);
		buff_del_symbol(g_term->buffer->iterator);
	}
}

void	handle_line_kill(u_int64_t key)
{
	if (key == CKILL && g_term->buffer->size > 0)
	{
		caret_move(g_term->buffer->size, D_LEFT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		clear_buffer(0);
	}
}
