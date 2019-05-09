/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector_parts.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:32:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/01 18:20:31 by vtarasiu         ###   ########.fr       */
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
** Returns malloc'ed char string of buffer contents
** not bigger than (8 * buffer_capacity) + 1 bytes
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

t_symbol		*buff_get_part_s(u_int64_t from_index, u_int64_t to_index)
{
	t_symbol		*array;

	if (to_index < from_index)
		return (NULL);
	if (to_index > g_buffer->size)
		to_index = g_buffer->size;
	array = ft_memalloc(sizeof(t_symbol) * (to_index - from_index + 1));
	ft_memcpy(array, g_buffer->array + from_index,
						sizeof(t_symbol) * (to_index - from_index));
	return (array);
}
