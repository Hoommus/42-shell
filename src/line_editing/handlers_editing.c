#include "line_editing.h"
#include "script_lang.h"

void	handle_backspace(int key)
{
	if (key == K_BSP && g_term->v_buffer->iterator > 0)
	{
		handle_left(K_LEFT);
		toggle_state(buff_char_at(g_term->v_buffer->iterator));
		buff_del_symbol(g_term->v_buffer->iterator);
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		buffer_redraw(-1);
	}
}

void	handle_del(int key)
{
	if (key == K_DEL && g_term->v_buffer->iterator < g_term->v_buffer->size)
	{
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		buffer_redraw(0);
		buff_del_symbol(g_term->v_buffer->iterator);
	}
}

void	handle_line_kill(int key)
{
	if (key == CKILL && g_term->v_buffer->size > 0)
	{
		load_caret_position(POS_PROMPT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		reset_buffer(0);
	}
}
