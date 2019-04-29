/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/19 13:08:18 by vtarasiu         ###   ########.fr       */
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

#include "twenty_one_sh.h"
#include "line_editing.h"

void			carpos_adjust_db(int by)
{
	int		i;

	if (by < 1)
		return ;
	i = 0;
	while (i < POS_CUSTOM2)
	{
		g_term->carpos_db[i].row -= by;
		i++;
	}
}

/*
** If distance is big, searches for first newline
** and counts physical coordinates from that character
**
** That's a shitload of '-1', isn't it?
**
** Also, 'penult' is short for 'penultimate'
*/

static void		left_hard(int dist, int *new_c, int *new_r)
{
	int		ult_nl;
	int		penult_nl;

	ult_nl = (int)g_term->buffer->iterator;
	while (dist)
	{
		ult_nl = ult_nl - 1;
		if (*new_c == 0 && buff_char_at_equals(ult_nl, "\n"))
		{
			penult_nl = buff_rchr("\n", ult_nl - 1);
			if (penult_nl < 0)
				penult_nl = -carpos_get(POS_PROMPT)->col - 1;
			*new_c = ult_nl - penult_nl - 1;
			*new_r -= *new_c > g_term->ws_col ? *new_c / g_term->ws_col : 1;
			*new_c = *new_c > g_term->ws_col ? *new_c % g_term->ws_col : *new_c;
		}
		else if (*new_c == 0)
		{
			*(new_c) = g_term->ws_col - 1;
			*new_r -= 1;
		}
		else
			*(new_c) -= 1;
		dist--;
	}
}

static void		right_hard(int distance, int *new_col, int *new_row)
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

/*
** caret_move - magically move caret in any desired direction.
**
** caret_move fully handles cursor movement on its own.
** Meaning it takes into account all newlines and window size before calculating
** final cursor position and moving it directly there via `cm' capability
*/

t_carpos		caret_move(int distance, enum e_direction direction)
{
	int			new_col;
	int			new_row;
	t_carpos	position;

	position.col = -1;
	position.row = -1;
	if (distance <= 0 || direction == D_NOWHERE)
		return (position);
	new_col = carpos_get(POS_CURRENT)->col;
	new_row = carpos_get(POS_CURRENT)->row;
	if (direction == D_LEFT)
		left_hard(distance, &new_col, &new_row);
	else if (direction == D_RIGHT)
		right_hard(distance, &new_col, &new_row);
	position.col = new_col;
	position.row = new_row;
	tputs(tgoto(tgetstr("cm", NULL), new_col, new_row), 1, &ft_putc);
	return (position);
}
