#include <twenty_one_sh.h>
#include "twenty_one_sh.h"
#include "line_editing.h"

void	buffer_redraw(int symbol_index)
{
	t_symbol	*buffer;
	t_symbol	tmp;

	symbol_index = 0;
	buffer = g_term->buffer->array;
	tmp = buffer[g_term->buffer->iterator];
	/* 
	if (symbol_index != -1)
		buffer[g_term->buffer->iterator].s ' ';
	if ((int64_t)ft_strlen(g_term->buffer) >= g_term->ws_col
		- get_caretpos(POS_PROMPT)->col)
	{
		save_caret_position_as(POS_CUSTOM1);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		ft_printf("%s", g_term->buffer + g_term->iterator);
		load_caret_position(POS_CUSTOM1);
	}
	buffer[g_term->buffer->iterator] = tmp;
	*/
}

void	buffer_visual_erase(int from_index)
{
	caret_move(g_term->buffer->iterator - from_index, D_LEFT);
	tputs(tgetstr("cd", NULL), 1, &ft_putc);
}
