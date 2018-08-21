# include "../../include/line_editing.h"

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

int		cursor_move(int distance, enum e_direction direction)
{
	if (direction == D_LEFT)
		return (tputs(tgetstr("le", NULL), distance, &ft_putc));
	else if (direction == D_RIGHT)
		return (tputs(tgetstr("nd", NULL), distance, &ft_putc));
	return (0);
}
