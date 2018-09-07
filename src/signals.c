/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:46:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/twenty_one_sh.h"
#include "../include/line_editing.h"

void	ignore(int sig)
{
	sig = 0;
	if (g_running_process == 0)
	{
		clear_buffer(0);
		ft_printf("\n");
		display_normal_prompt();
		g_term->input_state = NORMAL;
	}
}

void	resize(int sig)
{
	struct winsize size;

	if (sig == SIGWINCH)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
	}
}

void	setup_signal_handlers(void)
{
//	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, &ignore);
	signal(SIGTSTP, &ignore);
	signal(SIGWINCH, &resize);
}
