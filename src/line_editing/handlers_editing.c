#include "../../include/line_editing.h"
#include "../../include/script_lang.h"

void	handle_backspace(int key)
{
	if (key == K_BSP && g_term->iterator > 0)
	{
		handle_left(K_LEFT);
		if ((g_term->buffer[g_term->iterator] == '\''
				&& g_term->input_state == QUOTE)
			|| (g_term->buffer[g_term->iterator] == '"'
				&& g_term->input_state == DQUOTE)
			|| (g_term->buffer[g_term->iterator] == '`'
				&& g_term->input_state == BQUOTE))
		{
			g_term->input_state = NORMAL;
		}
		g_term->iterator -= delete_char_at(g_term->buffer, g_term->iterator) - 1;
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		redraw_buffer();
	}
}

void	handle_del(int key)
{
	if (key == K_DEL && ft_strlen(g_term->buffer + g_term->iterator) > 0)
	{
		delete_char_at(g_term->buffer, g_term->iterator);
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		redraw_buffer();
	}
}

void	handle_line_kill(int key)
{
	if (key == CKILL && ft_strlen(g_term->buffer) > 0)
	{
		load_caret_position(POS_PROMPT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		clear_buffer('\n');
	}
}
