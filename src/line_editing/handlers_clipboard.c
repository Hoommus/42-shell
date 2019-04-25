/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_clipboard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 20:56:19 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/22 20:56:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

void	handle_ctrl_y(union u_char key)
{
	if (key.lng == CKILL && g_term->buffer->size > 0)
	{
		// TODO: add paste_board
		caret_move(g_term->buffer->iterator, D_LEFT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		buff_clear(0);
	}
}

void	handle_ctrl_u(union u_char key)
{
	if (key.lng == CKILL && g_term->buffer->size > 0)
	{
		// TODO: add paste_board
		caret_move(g_term->buffer->iterator, D_LEFT);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
		buff_clear(0);
	}
}
