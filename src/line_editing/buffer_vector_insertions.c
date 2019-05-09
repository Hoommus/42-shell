/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_vector_insertions.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/01 16:50:31 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/01 17:42:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "buffer_works.h"

extern t_buffer		*g_buffer;

int					buff_insert_symbols_at(u_int64_t index, t_symbol *array)
{
	u_int64_t	size;

	size = 0;
	while (array[size].s[0] != '\0')
		size++;
	ft_memmove(g_buffer->array + index + size, g_buffer->array + index,
		sizeof(t_symbol) * size);
	ft_memcpy(array, g_buffer->array + index, sizeof(t_symbol) * size);
	g_buffer->iterator += size;
	g_buffer->size += size;
	return (0);
}
