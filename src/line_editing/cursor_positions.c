#include "../../include/line_editing.h"

t_carpos	*get_caretpos(enum e_position type)
{
	return (g_term->carpos_db + type);
}

t_carpos	*get_carpos(enum e_position type)
{
	return (g_term->carpos_db + type);
}

t_carpos	*save_caret_position_as(enum e_position type)
{
	update_caret_position(type);
	return (g_term->carpos_db + type);
}

/*
** Resets caret position to desired location
*/
t_carpos	*load_caret_position(enum e_position type)
{
	tputs(tgoto(tgetstr("cm", NULL), g_term->carpos_db[type].col - 1,
				g_term->carpos_db[type].row - 1), 1, &ft_putc);
	return (g_term->carpos_db + type);
}

/*
** Updates specified position in db via ANSI request code
*/
void		update_caret_position(enum e_position type)
{
	char		response[16];

	ft_printf("\x1b[6n");
	read(STDIN_FILENO, response, 16);
	if (ft_strchr(response, '[') && ft_strchr(response, ';'))
	{
		g_term->carpos_db[type].row =
				(short)ft_atoi(ft_strchr(response, '[') + 1);
		g_term->carpos_db[type].col =
				(short)ft_atoi(ft_strchr(response, ';') + 1);
	}
}
