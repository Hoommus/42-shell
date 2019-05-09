/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pasteboard_interface.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:15:18 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/03 18:59:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

static t_symbol		*g_pasteboard;

int					pb_paste(void)
{
	u_int64_t	i;

	i = -1;
	while (g_pasteboard && g_pasteboard[++i].s[0] != 0)
	{
		buff_insert_symbol_at(g_term->buffer->iterator, g_pasteboard + i);
		write(1, g_pasteboard[i].s, ft_strlen(g_pasteboard[i].s));
	}
	return (i == (u_int64_t)-1 ? 0 : i);
}

int					pb_copy(u_int64_t from, u_int64_t to)
{
	if (g_pasteboard)
		ft_memdel((void **)&g_pasteboard);
	g_pasteboard = buff_get_part_s(from, to);
	return (to - from);
}

int					pb_cut(u_int64_t from, u_int64_t to)
{
	if (g_pasteboard)
		ft_memdel((void **)&g_pasteboard);
	g_pasteboard = buff_get_part_s(from, to);
	buff_clear_part(from, to);
	return (to - from);
}

void				pb_clear(void)
{
	if (g_pasteboard)
		ft_memdel((void **)&g_pasteboard);
}
