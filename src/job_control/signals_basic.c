/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/27 14:18:34 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"
#include "shell_history.h"
#include "shell_job_control.h"

static void				tstp(int sig)
{
	sig = 0;
}

static void				handle_sigint(int sig)
{
	g_interrupt = sig;
}

static void				resize(int sig)
{
	struct winsize	size;

	if (sig == SIGWINCH)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
		carpos_update(POS_CURRENT);
	}
}

void					setup_signal_handlers(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(struct sigaction));
	action.__sigaction_u.__sa_handler = &tstp;
	sigaction(SIGTSTP, &action, NULL);
	action.__sigaction_u.__sa_handler = &handle_sigint;
	sigaction(SIGINT, &action, NULL);
	signal(SIGWINCH, &resize);
	signal(SIGPIPE, &tstp);
	signal(SIGCHLD, SIG_IGN);
}
