/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows_vertical.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/18 12:30:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"

void	handle_alt_up(union u_char key)
{
	int64_t		i;

	if (key.lng != K_ALT_UP)
		return ;
	i = g_term->buffer->iterator;
	while (i >= 0)
	{

	}
}

void	handle_alt_down(union u_char key)
{
	key.lng = 0;
}

