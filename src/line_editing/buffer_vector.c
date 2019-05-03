/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 21:11:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/02 20:20:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "buffer_works.h"
#include "twenty_one_sh.h"

t_buffer		*g_buffer;

void			init_buffer_vector(u_int64_t capacity)
{
	if (capacity <= 0)
		capacity = 1;
	g_buffer = (t_buffer *)ft_memalloc(sizeof(t_buffer));
	g_buffer->array = (t_symbol *)ft_memalloc(sizeof(t_symbol)
			* (capacity + 1));
	g_buffer->capacity = capacity;
	g_buffer->iterator = 0;
	g_term->buffer = g_buffer;
}

static void		reallocate_buffer_vector(void)
{
	t_symbol	*tmp;
	u_int64_t	sz;

	g_buffer->capacity += g_buffer->capacity / 2;
	tmp = (t_symbol *)ft_memalloc(sizeof(t_symbol) * g_buffer->capacity);
	sz = sizeof(t_symbol) * g_buffer->size;
	ft_memcpy(tmp, g_buffer->array, sz);
	free(g_buffer->array);
	g_buffer->array = tmp;
}

/*
** TODO: Add visual size table lookup
*/

int				buff_insert_symbol_at(u_int64_t index, t_symbol *symbol)
{
	if (g_buffer->size >= g_buffer->capacity - 2)
		reallocate_buffer_vector();
	if (index == g_buffer->size)
		g_buffer->array[g_buffer->iterator] = *symbol;
	else
	{
		ft_memmove(g_buffer->array + index + 1, g_buffer->array + index,
					(g_buffer->size - index) * sizeof(t_symbol));
		ft_memcpy(g_buffer->array + index, symbol, sizeof(t_symbol));
	}
	g_buffer->size += 1;
	g_buffer->iterator += 1;
	return (OK);
}

int				buff_insert_single_at(u_int64_t index, const char str[8])
{
	t_symbol	symbol;

	ft_memcpy(symbol.s, str, 4);
	symbol.visual_size = 1;
	return (buff_insert_symbol_at(index, &symbol));
}

int				buff_insert_string_at(u_int64_t index, const char *string)
{
	u_int64_t	i;
	u_int64_t	limit;
	u_int64_t	jump;
	char		arr[8];

	if (string == NULL)
		return (1);
	limit = ft_utf_strlen((char *)string);
	if (limit + g_buffer->size > g_buffer->capacity)
		reallocate_buffer_vector();
	i = 0;
	while (*string && i < limit)
	{
		ft_bzero(arr, sizeof(char) * 8);
		jump = 1 + utf_body_size(*string);
		buff_insert_single_at(index + i, ft_memcpy(arr, string, jump));
		string += jump;
		i++;
	}
	return (OK);
}
