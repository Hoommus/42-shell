#include "../../include/twenty_one_sh.h"
#include "../../include/line_editing.h"

void			redraw_buffer(void)
{
	if (g_term->buffer[g_term->iterator] != '\0'
		&& (int64_t)ft_strlen(g_term->buffer) > g_term->ws_col
										- get_caretpos(POS_PROMPT)->col)
	{
		save_caret_position_as(POS_LAST);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		ft_printf("%s", g_term->buffer + g_term->iterator);
		load_caret_position(POS_LAST);
	}
}

