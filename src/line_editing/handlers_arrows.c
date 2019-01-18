/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_arrows.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/16 19:06:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "shell_history.h"

// TODO: add partial command completion from history

void	handle_up(union u_char key)
{
	extern t_history		*g_history;
	struct s_history_entry	*entry;

	if (key.lng == K_UP && (!g_term->buffer->iterator || !g_term->buffer->size
		|| g_term->buffer->iterator == g_term->buffer->size)
		&& g_history->iterator > 0)
	{
		while ((entry = history_get_entry(--g_history->iterator)))
			if (ft_strcmp(entry->command, buff_get_part(0, UINT64_MAX)))
				break ;
		if (!entry)
			return ;
		caret_move(g_term->buffer->iterator, D_LEFT);
		buff_clear(0);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		buff_insert_string_at(0, entry->command);
		ft_printf("%s", entry->command);
	}
}

void	handle_down(union u_char key)
{
	extern t_history		*g_history;
	struct s_history_entry	*entry;

	if (key.lng == K_DOWN && history_get_entry(g_history->iterator) != NULL)
	{
		while ((entry = history_get_entry(++g_history->iterator)))
			if (ft_strcmp(entry->command, buff_get_part(0, UINT64_MAX)))
				break ;
		caret_move(g_term->buffer->iterator, D_LEFT);
		buff_clear(0);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		if (!entry)
			return ;
		buff_insert_string_at(0, entry->command);
		ft_printf("%s", entry->command);
	}
}

void	handle_left(union u_char key)
{
	if (key.lng == K_LEFT && g_term->buffer->iterator > 0)
	{
		caret_move(1, D_LEFT);
		g_term->buffer->iterator--;
	}
}

void	handle_right(union u_char key)
{
	if (key.lng == K_RIGHT && g_term->buffer->iterator < g_term->buffer->size
		&& ft_strcmp(buff_char_at(g_term->buffer->iterator), "\0") != 0)
	{
		caret_move(1, D_RIGHT);
		g_term->buffer->iterator++;
	}
}
