/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_editing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 13:45:39 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/22 20:40:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

void	handle_backspace(union u_char key)
{
	if (key.lng == K_BSP && g_term->buffer->iterator > 0)
	{
		if ((g_term->input_state == STATE_QUOTE ||
			g_term->input_state == STATE_DQUOTE ||
			g_term->input_state == STATE_BQUOTE ||
			g_term->input_state == STATE_ESCAPED) &&
			buff_char_at_equals(g_term->buffer->iterator - 1, "\n"))
			return ;
		handle_left((union u_char){K_LEFT});
		toggle_state(buff_char_at(g_term->buffer->iterator));
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
			toggle_state(buff_char_at(g_term->buffer->iterator));
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
