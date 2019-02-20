#include "line_editing.h"

t_carpos	*carpos_get(enum e_position type)
{
	return (g_term->carpos_db + type);
}

t_carpos	*carpos_save_as(enum e_position type)
{
	carpos_update(type);
	return (g_term->carpos_db + type);
}

/*
** Resets caret position to desired location
*/
t_carpos	*carpos_load(enum e_position type)
{
	tputs(tgoto(tgetstr("cm", NULL), g_term->carpos_db[type].col,
				g_term->carpos_db[type].row), 1, &ft_putc);
	return (g_term->carpos_db + type);
}

/*
** Updates specified position in db via ANSI request code
*/
t_carpos	*carpos_update(enum e_position type)
{
	char		response[16];

	write(2, "\x1b[6n", 4);
	read(STDIN_FILENO, response, 16);
	if (ft_strchr(response, '[') && ft_strchr(response, ';'))
	{
		g_term->carpos_db[type].row =
				(short)(ft_atoi(ft_strchr(response, '[') + 1) - 1);
		g_term->carpos_db[type].col =
				(short)(ft_atoi(ft_strchr(response, ';') + 1) - 1);
	}
	return (g_term->carpos_db + type);
}
