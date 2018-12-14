/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/14 17:51:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** This is Cursor Control to Major Tom
** Your circuit's dead, there's something wrong
** Can you hear me, Major Tom?
** Can you hear me, Major Tom?
** Can you hear me, Major Tom?
** Can you~
*/

#include "line_editing.h"

#include <assert.h>
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

/*
** If distance is big, searches for first newline
** and counts physical coordinates from that character
**
** So many `-1', isn't it?
*/

void		left_hard(int dist, int *new_col, int *new_row)
{
	int		ult_nl;
	int		penult_nl;

	dist += 1;
	ult_nl = (int) g_term->buffer->iterator;
	while (--dist)
	{
		ult_nl = ult_nl - 1;
		if (*new_col == 0 && buff_char_at_equals(ult_nl, "\n"))
		{
			penult_nl = buff_chroff(g_term->buffer, "\n", ult_nl - 1);
			if (penult_nl < 0)
				penult_nl = -carpos_get(POS_PROMPT)->col - 1;
			*new_col = ult_nl - penult_nl - 1;
			*new_row -= *new_col > g_term->ws_col ? *new_col / g_term->ws_col : 1;
			*new_col = *new_col > g_term->ws_col ? *new_col % g_term->ws_col : *new_col;
		}
		else if (*new_col == 0)
			*(new_col) = g_term->ws_col - 1;
		else
			*(new_col) -= 1;
	}
}

void		right_hard(int distance, int *new_col, int *new_row)
{
	int		i;

	i = 0;
	while (i < distance)
	{
		if (*new_col >= g_term->ws_col - 1 ||
			buff_char_at_equals(g_term->buffer->iterator + i, "\n"))
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

	if (distance == 0 || direction == D_NOWHERE)
		return (0);
	new_col = carpos_get(POS_CURRENT)->col;
	new_row = carpos_get(POS_CURRENT)->row;
	if (direction == D_LEFT)
		left_hard(distance, &new_col, &new_row);
	else if (direction == D_RIGHT)
		right_hard(distance, &new_col, &new_row);
	tputs(tgoto(tgetstr("cm", NULL), new_col, new_row), 1, &ft_putc);
	return (0);
}
