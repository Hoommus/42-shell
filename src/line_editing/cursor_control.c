#include "../../include/line_editing.h"

int			ft_strchr_back(const char *str, char c, int i)
{
	while (i >= 0)
	{
		if (str[i] == c)
			break;
		i--;
	}
	return (i);
}

void		adjust_carpos_db(void)
{
	int		i;

	i = 0;
	while (i < POS_CUSTOM2)
	{
		g_term->carpos_db[i].row -= 1;
		i++;
	}
}

void		left_hard(int dist, int *new_col, int *new_row)
{
	int		tmp;

	while (dist)
	{
		if (*new_col == 0)
		{
			*(new_row) -= 1;
			if (g_term->buffer[g_term->iterator - dist] == '\n')
			{
				tmp = ft_strchr_back(g_term->buffer, '\n',
									g_term->iterator - dist);
				*(new_col) = tmp - (tmp % g_term->ws_col) * g_term->ws_col;
			}
			else
				*(new_col) = g_term->ws_col - 1;
		}
		else
			*(new_col) -= 1;
		dist--;
	}
}

void		right_hard(int distance, int *new_col, int *new_row)
{
	int		i;

	i = 0;
	while (i < distance)
	{
		if (*new_col >= g_term->ws_col - 1
			|| g_term->buffer[g_term->iterator + i] == '\n')
		{
			*(new_row) += 1;
			*(new_col) = 0;
		}
		else
			*(new_col) += 1;
		i++;
	}
}

int			caret_move(int distance, enum e_direction direction)
{
	int		new_col;
	int		new_row;

	new_col = get_caretpos(POS_CURRENT)->col;
	new_row = get_caretpos(POS_CURRENT)->row;
	if (direction == D_LEFT)
		left_hard(distance, &new_col, &new_row);
	else if (direction == D_RIGHT)
		right_hard(distance, &new_col, &new_row);
	tputs(tgoto(tgetstr("cm", NULL), new_col, new_row), 1, &ft_putc);
	return (0);
}
