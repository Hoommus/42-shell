/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_positions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 21:08:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/25 12:51:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"
#include "line_editing.h"
#include <errno.h>

t_carpos	*carpos_get(enum e_position type)
{
	return (g_term->carpos_db + type);
}

t_carpos	*carpos_save_as(enum e_position type)
{
	carpos_update(type);
	return (g_term->carpos_db + type);
}

/*
** Resets caret position to desired location
*/

t_carpos	*carpos_load(enum e_position type)
{
	if (g_term->input_state != STATE_NON_INTERACTIVE &&
		g_term->input_state != STATE_JOB_IN_FG)
		tputs(tgoto(tgetstr("cm", NULL), g_term->carpos_db[type].col,
				g_term->carpos_db[type].row), 1, &ft_putc);
	return (g_term->carpos_db + type);
}

/*
** Updates specified position in db via ANSI request code
*/

t_carpos	*carpos_update(enum e_position type)
{
	char		response[17];
	int			log;

	if (g_term->tty_fd != -1 && g_term->input_state != STATE_NON_INTERACTIVE
		&& g_term->input_state != STATE_JOB_IN_FG)
	{
		log = open(LOG_FILE, O_WRONLY | O_APPEND);
		if (write(2, "\x1b[6n", 4) == -1)
			ft_dprintf(log, "Got -1 from write: %s\n", strerror(errno));
		response[16] = 0;
		if (read(STDIN_FILENO, response, 10) == -1)
			ft_dprintf(log, "Got -1 from read: %s\n", strerror(errno));
		if (ft_strchr(response, '[') && ft_strchr(response, ';'))
		{
			g_term->carpos_db[type].row =
				(short)(ft_atoi(ft_strchr(response, '[') + 1) - 1);
			g_term->carpos_db[type].col =
				(short)(ft_atoi(ft_strchr(response, ';') + 1) - 1);
		}
		close(log);
	}
	return (g_term->carpos_db + type);
}
