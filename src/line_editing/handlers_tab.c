/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_tab.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 16:39:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/05 18:59:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>
#include <line_editing.h>
#include "buffer_works.h"
#include "acompl.h"

void							handle_tab(union u_char key)
{
	char		*str;
	char		buf[1024];
	size_t		i;
	bool		is_cmd;

	if (g_term->input_state == STATE_NORMAL)
		recheck_state(0);
	if (key.lng != '\t')
		return ;
	is_cmd = 0;
	ft_bzero(buf, sizeof(buf));
	str = buff_get_part(0, g_term->buffer->iterator);
	i = g_term->buffer->iterator;
	while (i >= 0)
	{
		if (str[i] == ';' || str[i] == '\n' || str[i] == '&')
		{
			is_cmd = true;
			break ;
		}
		else if (str[i] == ' ')
			break ;
		i--;
	}
	acompl(str + i + 1, buf, 1024 * sizeof(char), is_cmd);

}
