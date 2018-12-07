/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector_tools1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/19 15:35:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/19 15:35:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "buffer_works.h"

extern t_buffer	*g_buffer;

/*
** bzero'es buffer starting from 'from_index' and to the end
*/

void			clear_buffer(u_int64_t from_index)
{
	struct s_symbol	*start;

	start = g_buffer->array + from_index;
	ft_bzero(start, sizeof(t_symbol) * (g_buffer->capacity - from_index));
	g_buffer->size = from_index;
	if (g_buffer->iterator > from_index)
		g_buffer->iterator = from_index;
}

int				buff_del_symbol(u_int64_t index)
{
	if (index == g_buffer->size)
	{
		ft_bzero(g_buffer->array + g_buffer->iterator, sizeof(t_buffer));
		g_buffer->size--;
		return (0);
	}
	if (index > g_buffer->size)
		return (-1);
	ft_memmove(g_buffer->array + index, g_buffer->array + index + 1,
			(g_buffer->size - index) * sizeof(t_symbol));
	g_buffer->size--;
	return (0);
}

/*
**
*/
char			*buff_get_part(u_int64_t start, u_int64_t end)
{
	char		*buffer;
	char		*stripped;
	u_int64_t	i;
	u_int64_t	j;

	if (end > g_buffer->capacity)
		end = g_buffer->size;
	i = start;
	j = 0;
	buffer = ft_memalloc(sizeof(char) * ((end - start) * 8 + 1));
	while (i < end && ft_strlen(g_buffer->array[i].s) > 0)
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

char			*buff_get_string(void)
{
	return (NULL);
}
