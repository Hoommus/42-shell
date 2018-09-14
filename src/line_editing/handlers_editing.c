#include "../../include/line_editing.h"
#include "../../include/script_lang.h"

void	handle_backspace(int key)
{
	if (key == K_BSP && g_term->iterator > 0)
	{
		handle_left(K_LEFT);
		if (g_term->buffer[g_term->iterator] == '\'')
			toggle_quote();
		else if (g_term->buffer[g_term->iterator] == '"')
			toggle_dquote();
		else if (g_term->buffer[g_term->iterator] == '`')
			toggle_bquote();
		else if (g_term->buffer[g_term->iterator] == '\\')
			toggle_escaped();
		g_term->iterator -= delete_char_at(g_term->buffer, g_term->iterator) - 1;
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		redraw_buffer(-1);
	}
}

void	handle_del(int key)
{
	if (key == K_DEL && ft_strlen(g_term->buffer + g_term->iterator) > 0)
	{
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		redraw_buffer(0);
		delete_char_at(g_term->buffer, g_term->iterator);
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
