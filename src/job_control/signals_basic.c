/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/03 19:12:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"
#include "shell_job_control.h"

static void				tstp(int sig)
{
	sig = 0;
}

static void				handle_sigint(int sig)
{
	g_interrupt = sig;
	g_term->last_status = 1;
}

static void				resize(int sig)
{
	struct winsize	size;

	if (sig == SIGWINCH && jc_get()->active_jobs == NULL)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
		carpos_update(POS_CURRENT);
	}
}

void					sigttou(int sig)
{
	tcsetpgrp(0, g_term->shell_pgid);
	sig = 0;
}

void					setup_signal_handlers(void)
{
	struct sigaction	action;
	sigset_t			set;

	ft_bzero(&action, sizeof(struct sigaction));
	action.sa_handler = &handle_sigint;
	sigaction(SIGINT, &((struct sigaction){
		.sa_flags = 0,
		.sa_mask = {0},
		.sa_handler = &handle_sigint
	}), NULL);
	sigaction(SIGTSTP, &((struct sigaction){
		.sa_flags = SA_RESTART,
		.sa_handler = &tstp}), NULL);
	sigchild_set_handler();
	signal(SIGWINCH, &resize);
	signal(SIGPIPE, &tstp);
	signal(SIGQUIT, &tstp);
	signal(SIGTTOU, &sigttou);
	signal(SIGTTIN, &sigttou);
	sigemptyset(&set);
	sigaddset(&set, SIGTTOU);
	sigaddset(&set, SIGTTIN);
}
