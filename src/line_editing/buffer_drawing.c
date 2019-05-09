/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_drawing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 18:13:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/02 16:41:25 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

/*
** TODO: Consider adding capability to redraw starting at some position in buff
*/

void	buffer_redraw(void)
{
	char		*string;

	carpos_update(POS_CUSTOM1);
	tputs(tgetstr("cd", NULL), 1, &ft_putc);
	string = buff_get_part(g_term->buffer->iterator, -1);
	ft_printf("%s", string);
	free(string);
	carpos_load(POS_CUSTOM1);
}

void	buffer_visual_erase(int from_index)
{
	caret_move(g_term->buffer->iterator - from_index, D_LEFT);
	tputs(tgetstr("cd", NULL), 1, &ft_putc);
}
