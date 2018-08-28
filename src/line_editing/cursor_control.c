#include "../../include/line_editing.h"

enum e_direction	choose_direction(int key)
{
	if (key == K_UP)
		return (D_UP);
	else if (key == K_DOWN)
		return (D_DOWN);
	else if (key == K_LEFT)
		return (D_LEFT);
	else if (key == K_RIGHT)
		return (D_RIGHT);
	return (D_NOWHERE);
}

int					cursor_move(int distance, enum e_direction direction)
{
	if (distance > 1)
	{

	}
	if (direction == D_LEFT)
		return (tputs(tgetstr("le", NULL), distance, &ft_putc));
	else if (direction == D_RIGHT)
		return (tputs(tgetstr("nd", NULL), distance, &ft_putc));
	return (0);
}

void				update_cursor_position(void)
{
	char			response[16];

	ft_printf("\033[6n");
	read(STDIN_FILENO, response, 16);
	if (ft_strchr(response, '[') && ft_strchr(response, ';'))
	{
		g_term->cursor_row = ft_atoi(ft_strchr(response, '[') + 1);
		g_term->cursor_col = ft_atoi(ft_strchr(response, ';') + 1);
	}
}
