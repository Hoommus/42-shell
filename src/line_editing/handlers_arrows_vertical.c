/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows_vertical.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/22 15:14:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"

#include <assert.h>
void	handle_alt_up(union u_char key)
{
	t_carpos	start;
	t_carpos	tmp;
	int64_t		i;
//	int64_t		nl;

	if (key.lng != K_ALT_UP)
		return ;
	start = *carpos_update(POS_CUSTOM2);
	i = 0;
	while (g_term->buffer->iterator - i > 0)
	{
		tmp = caret_move(1, D_LEFT);
		i++;
		if ((tmp.col <= start.col && tmp.row < start.row))
			break ;
	}
	g_term->buffer->iterator -= i;
//	nl = buff_chroff(g_term->buffer, "\n", g_term->buffer->iterator);
//	if (g_term->buffer->iterator - nl < start.col)
//	caret_move(start.col + 1, D_LEFT);
//	i = start.col + 1;
//	tmp = *carpos_update(POS_CUSTOM2);
//	if (tmp.col > start.col && tmp.row < start.row)
//	{
//		write_at(0, 0, "tmp.col > start.col");
//		caret_move(start.col + 1, D_LEFT);
//		i += g_term->ws_col - start.col - 1;
//	}
//	g_term->buffer->iterator -= i;
	assert(g_term->buffer->iterator >= 0);
}

void	handle_alt_down(union u_char key)
{
	int64_t		i;

	i = g_term->buffer->iterator;
//	while (true)
//	{
//		g_term->buffer->iterator += 1;
//
//	}
	key.lng = 0;
}

