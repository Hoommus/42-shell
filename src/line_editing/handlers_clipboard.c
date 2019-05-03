/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_clipboard.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 20:56:19 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/02 20:45:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"

void	buffer_redraw_i(u_int64_t from_index)
{
	char		*string;

	tputs(tgetstr("cd", NULL), 1, &ft_putc);
	string = buff_get_part(from_index, -1);
	write(1, string, ft_strlen(string));
	free(string);
}

void	handle_ctrl_y(union u_char key)
{
	if (key.lng == K_CTRL_Y)
	{
		if (pb_paste() == 0)
			return ;
		buffer_redraw();
	}
}

void	handle_ctrl_u(union u_char key)
{
	if (key.lng == CKILL && g_term->buffer->size > 0)
	{
		pb_copy(0, UINT64_MAX);
		caret_move(g_term->buffer->iterator, D_LEFT);
		buff_clear(0);
		tputs(tgetstr("cd", NULL), 1, &ft_putc);
	}
}
