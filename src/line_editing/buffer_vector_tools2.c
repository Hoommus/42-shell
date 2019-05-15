/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector_tools2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 11:54:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/22 21:10:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buffer_works.h"

extern t_buffer	*g_buffer;

/*
** Returns last desired char's offset in buffer searching from index 'i' to 0
** If nothing found, returns -1
*/

int64_t			buff_rchr(const char *c, int64_t i)
{
	if (i > (int64_t)g_buffer->capacity)
		return (0);
	while (i >= 0)
	{
		if (ft_strcmp(g_buffer->array[i].s, c) == 0)
			break ;
		i--;
	}
	return (i);
}

char			*buff_char_at(const u_int64_t index)
{
	return (g_buffer->array[index].s);
}

int				buff_char_at_equals(const u_int64_t index, const char *c)
{
	if (c == NULL || ft_strlen((char *)c) > 5 || index >= g_buffer->capacity)
		return (false);
	return (!ft_strcmp(g_buffer->array[index].s, c));
}

int				buff_char_at_equals_any(const u_int64_t index, const char *c)
{
	size_t		size;
	size_t		i;

	if (c == NULL || ft_strlen(g_buffer->array[index].s) == 0)
		return (false);
	i = 0;
	size = ft_utf_strlen(c);
	while (i < size && ft_strlen(c))
	{
		if (ft_strlen(c) > 0 && ft_strncmp(g_buffer->array[index].s, c,
			ft_strlen(g_buffer->array[index].s)) == 0)
			return (true);
		c += 1 + utf_body_size(*c);
		i++;
	}
	return (false);
}
