/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows_vertical.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:19:50 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

void	handle_alt_up(union u_char key)
{
	t_carpos	start;
	t_carpos	tmp;
	int64_t		i;

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
}

void	handle_alt_down(union u_char key)
{
	int64_t		i;

	i = g_term->buffer->iterator;
	key.lng = 0;
}
