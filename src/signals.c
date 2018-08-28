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

void	ignore(int sig)
{
	sig = 0;
	if (g_running_process == 0)
	{
		clear_buffer();
		ft_printf("\n");
		display_normal_prompt();
	}
}

void	resize(int sig)
{
	struct winsize size;

	if (sig == SIGWINCH)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->term_rows = size.ws_row;
		g_term->term_cols = size.ws_col;
	}
}

void	setup_signal_handlers(void)
{
//	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, &ignore);
	signal(SIGTSTP, &ignore);
	signal(SIGWINCH, &resize);
}
