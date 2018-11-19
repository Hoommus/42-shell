/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/19 16:40:57 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "shell_history.h"

void	handle_up(u_int64_t key)
{
	extern t_history		*g_history;
	struct s_history_entry	*entry;

	if (key == K_UP && (!g_term->buffer->iterator || !g_term->buffer->size
		|| g_term->buffer->iterator == g_term->buffer->size)
		&& g_history->iterator > 0)
	{
		entry = history_get_entry(--g_history->iterator);
		if (!entry)
			return ;
		caret_move(g_term->buffer->iterator, D_LEFT);
		clear_buffer(0);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		insert_string_at(0, entry->command);
		ft_printf("%s", entry->command);
	}
}

void	handle_down(u_int64_t key)
{
	extern t_history		*g_history;
	struct s_history_entry	*entry;

	if (key == K_DOWN && history_get_entry(g_history->iterator) != NULL)
	{
		entry = history_get_entry(++g_history->iterator);
		caret_move(g_term->buffer->iterator, D_LEFT);
		clear_buffer(0);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		if (!entry)
			return ;
		insert_string_at(0, entry->command);
		ft_printf("%s", entry->command);
	}
}

void	handle_left(u_int64_t key)
{
	if (key == K_LEFT && g_term->buffer->iterator > 0
		&& ft_strcmp(buff_char_at(g_term->buffer->iterator - 1), "\n") != 0)
	{
		caret_move(1, D_LEFT);
		g_term->buffer->iterator--;
	}
}

void	handle_right(u_int64_t key)
{
	if (key == K_RIGHT && g_term->buffer->iterator < g_term->buffer->size
		&& ft_strcmp(buff_char_at(g_term->buffer->iterator), "\0") != 0)
	{
		caret_move(1, D_RIGHT);
		g_term->buffer->iterator++;
	}
}
