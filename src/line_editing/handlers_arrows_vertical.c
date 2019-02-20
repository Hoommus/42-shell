/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows_vertical.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/18 18:18:04 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"

void	handle_alt_up(union u_char key)
{
	t_carpos	*start;
	int64_t		i;
	bool		did_line_change;

	if (key.lng != K_ALT_UP)
		return ;
	start = carpos_update(POS_CUSTOM2);
	did_line_change = false;
	i = g_term->buffer->iterator;
	while (i >= 0)
	{
		if (buff_char_at_equals(i, "\n"))
			did_line_change = true;
	}
}

void	handle_alt_down(union u_char key)
{
	key.lng = 0;
}

