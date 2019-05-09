/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_editing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 13:45:39 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/06 15:53:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

void	handle_backspace(union u_char key)
{
	if (key.lng == K_BSP && g_term->buffer->iterator > 0)
	{
		if ((g_term->input_state > STATE_NON_INTERACTIVE) &&
			buff_char_at_equals(g_term->buffer->iterator - 1, "\n"))
			return ;
		handle_left((union u_char){K_LEFT});
		buff_del_symbol(g_term->buffer->iterator);
		tputs(tgetstr("dc", NULL), 1, &ft_putc);
		buffer_redraw();
	}
}

void	handle_del(union u_char key)
{
	if (key.lng == K_DEL)
	{
		if (g_term->buffer->iterator <= g_term->buffer->size)
			tputs(tgetstr("dc", NULL), 1, &ft_putc);
		if (g_term->buffer->iterator < g_term->buffer->size)
		{
			buff_del_symbol(g_term->buffer->iterator);
			buffer_redraw();
		}
	}
}

void	handle_home(union u_char key)
{
	if (key.lng == K_HOME)
	{
		caret_move(g_term->buffer->iterator, D_LEFT);
		g_term->buffer->iterator = 0;
	}
}

void	handle_end(union u_char key)
{
	if (key.lng == K_END)
	{
		caret_move(g_term->buffer->size - g_term->buffer->iterator, D_RIGHT);
		g_term->buffer->iterator = g_term->buffer->size;
	}
}
