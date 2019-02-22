/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector_tools1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:35:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/22 15:30:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "buffer_works.h"

extern t_buffer	*g_buffer;

void			buff_clear_part(u_int64_t from_index, u_int64_t to_index)
{
	struct s_symbol	*start;
	u_int64_t		region_size;

	start = g_buffer->array + from_index;
	if (to_index > g_buffer->capacity)
		to_index = g_buffer->capacity;
	region_size = to_index - from_index;
	ft_bzero(start, sizeof(t_symbol) * region_size);
	g_buffer->size = g_buffer->size - region_size;
	if (g_buffer->iterator > from_index)
		g_buffer->iterator = from_index;
}

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

/*
**
*/
char			*buff_get_part(u_int64_t from_index, u_int64_t to_index)
{
	char		*buffer;
	char		*stripped;
	u_int64_t	i;
	u_int64_t	j;

	if (to_index < from_index)
		return (NULL);
	if (to_index > g_buffer->capacity)
		to_index = g_buffer->size;
	i = from_index;
	j = 0;
	buffer = ft_memalloc(sizeof(char) * ((to_index - from_index) * 8 + 1));
	while (i < to_index && ft_strlen(g_buffer->array[i].s) > 0)
	{
		ft_memcpy(buffer + j, g_buffer->array[i].s,
					ft_strlen(g_buffer->array[i].s));
		j += ft_strlen(g_buffer->array[i].s);
		i++;
	}
	stripped = ft_strdup(buffer);
	free(buffer);
	return (stripped);
}
