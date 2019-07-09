/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_tab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 16:39:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/06 18:26:43 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"
#include "buffer_works.h"
#include "acompl.h"

void							handle_tab(union u_char key)
{
	char			buf[1025];
	char			buf2[1025];
	size_t			offset;
	int64_t			rchr;
	enum e_acompl	state;

	if (key.lng != '\t')
		return ;
	rchr = buff_rchr("\n", g_term->buffer->iterator);
	acompl_update_state(rchr == -1 ? 0 : rchr, buf, 1024);
	if (!buf[0] || g_term->acompl_state == AC_NONE)
		return ;
	offset = ft_strlen(buf);
	state = acompl(buf, buf2, 1024 * sizeof(char),
		g_term->acompl_state == AC_COMMAND);
	if (state != ACOMPL_ERROR && state != ACOMPL_NOTHING &&
		ft_strcmp(buf, buf2))
	{
		buff_insert_string_at(g_term->buffer->iterator, buf2 + offset);
		buffer_redraw_i(g_term->buffer->iterator - (ft_strlen(buf2) - offset));
	}
}
