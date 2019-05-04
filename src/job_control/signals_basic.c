/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/04 16:09:49 by vtarasiu         ###   ########.fr       */
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

	if (sig == SIGWINCH && jc_get()->queue_size == 0)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
		carpos_update(POS_CURRENT);
	}
}

void					sigchild_alt(int sig, siginfo_t *info,
										void *smthng)
{
	t_job	*list;
	int		status;

	list = jc_get()->job_queue;
	while (list)
	{
		if (list->pid == info->si_pid &&
			(WIFEXITED(info->si_status) || WIFSIGNALED(info->si_status)))
		{
			list->wexitstatus = WEXITSTATUS(status);
			list->state = JOB_TERMINATED;
			break ;
		}
		list = list->next;
	}
	sig = 0;
	smthng = 0;
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
	signal(SIGCHLD, SIG_DFL);
}
