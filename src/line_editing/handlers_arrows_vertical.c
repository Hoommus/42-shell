/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows_vertical.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/15 13:13:01 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

void	handle_alt_up(union u_char key)
{
	u_int64_t		threshold;
	u_int64_t		i;

	threshold = g_term->ws_col;
	if (key.lng != K_ALT_UP)
		return ;
	carpos_update(POS_CUSTOM2);
	carpos_update(POS_CURRENT);
	i = 0;
	while (i < threshold && g_term->buffer->iterator != 0)
	{
		caret_move(1, D_LEFT);
		g_term->buffer->iterator--;
		if ((carpos_update(POS_CURRENT)->col <= carpos_get(POS_CUSTOM2)->col &&
				carpos_get(POS_CURRENT)->row < carpos_get(POS_CUSTOM2)->row))
			return ;
		i++;
	}
}

void	handle_alt_down(union u_char key)
{
	const u_int64_t	threshold = g_term->ws_col;
	u_int64_t		i;

	if (key.lng != K_ALT_DOWN)
		return ;
	if (g_term->buffer->size - g_term->buffer->iterator < threshold)
		handle_home((union u_char){.lng = K_END});
	carpos_update(POS_CUSTOM2);
	i = 0;
	while (i < threshold && g_term->buffer->iterator != g_term->buffer->size)
	{
		caret_move(1, D_RIGHT);
		g_term->buffer->iterator++;
		if (carpos_update(POS_CURRENT)->col == carpos_get(POS_CUSTOM2)->col
			|| (carpos_get(POS_CURRENT)->col > carpos_get(POS_CUSTOM2)->col &&
				carpos_get(POS_CURRENT)->row > carpos_get(POS_CUSTOM2)->row))
			return ;
		i++;
	}
}
