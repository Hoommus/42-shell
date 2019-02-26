/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows_mods.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 15:19:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/22 13:18:46 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

#include <assert.h>
static bool		is_buffer_symbol_at_index_wsp(int64_t index)
{
	return (buff_char_at_equals(index, " ") ||
			buff_char_at_equals(index, "\t") ||
			buff_char_at_equals(index, "\n") ||
			buff_char_at_equals(index, "\r") ||
			buff_char_at_equals(index, "\a"));
}

void	handle_alt_left(union u_char key)
{
	int64_t			i;

	if (key.lng != K_ALT_LEFT)
		return ;
	i = g_term->buffer->iterator;
	if (!is_buffer_symbol_at_index_wsp(g_term->buffer->iterator))
		i--;
	while (i >= 0 && (is_buffer_symbol_at_index_wsp(i)))
		i--;
	while (i >= 0)
	{
		if (is_buffer_symbol_at_index_wsp(i))
			break ;
		i--;
	}
	caret_move(ABS(g_term->buffer->iterator - i) - 1, D_LEFT);
	g_term->buffer->iterator = (u_int64_t)(i + 1);
}

void	handle_alt_right(union u_char key)
{
	int64_t			i;

	if (key.lng != K_ALT_RIGHT)
		return ;
	i = g_term->buffer->iterator;
	if (is_buffer_symbol_at_index_wsp(g_term->buffer->iterator))
		while (i < (int64_t)g_term->buffer->size && is_buffer_symbol_at_index_wsp(i))
			i++;
	while (i < (int64_t)g_term->buffer->size)
	{
		if (is_buffer_symbol_at_index_wsp(i))
			break ;
		i++;
	}
	caret_move(ABS(i - g_term->buffer->iterator), D_RIGHT);
	g_term->buffer->iterator = i;
}

void	handle_ctrl_w(union u_char key)
{
	int64_t				i;

	if (key.lng != K_CTRL_W)
		return ;
	i = g_term->buffer->iterator - 1;
	while (i >= 0 && is_buffer_symbol_at_index_wsp(i))
		buff_del_symbol(i--);
	while (i >= 0)
	{
		if (is_buffer_symbol_at_index_wsp(i))
		{
			buff_del_symbol(i);
			break ;
		}
		toggle_state(buff_char_at(i));
		buff_del_symbol(i--);
	}
	if (i == -1)
		caret_move(ABS(g_term->buffer->iterator - i) - 1, D_LEFT);
	else
		caret_move(ABS(g_term->buffer->iterator - i), D_LEFT);
	g_term->buffer->iterator = i == -1 ? 0 : i;
	buffer_redraw();
}
