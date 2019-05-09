/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector_tools1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:35:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/01 16:34:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "buffer_works.h"

extern t_buffer	*g_buffer;

/*
** bzero'es buffer starting from 'from_index' and to the end
*/

void			buff_clear(u_int64_t from_index)
{
	struct s_symbol	*start;

	start = g_buffer->array + from_index;
	ft_bzero(start, sizeof(t_symbol) * (g_buffer->capacity - from_index));
	g_buffer->size = from_index;
	if (g_buffer->iterator > from_index)
		g_buffer->iterator = from_index;
}

int				buff_del_symbol(u_int64_t at_index)
{
	if (at_index == g_buffer->size)
	{
		ft_bzero(g_buffer->array + g_buffer->iterator, sizeof(t_buffer));
		if (g_buffer->size != 0)
			g_buffer->size--;
		return (0);
	}
	if (at_index > g_buffer->size || at_index == (u_int64_t)-1)
		return (-1);
	ft_memmove(g_buffer->array + at_index, g_buffer->array + at_index + 1,
			(g_buffer->size - at_index) * sizeof(t_symbol));
	g_buffer->size--;
	return (0);
}
