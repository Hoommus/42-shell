/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/25 12:41:42 by vtarasiu         ###   ########.fr       */
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

void					sigquit(int sig)
{
	sig = 0;
}

void					sigchild(int sig, siginfo_t *info, void *ignore)
{
	pid_t	pgid;
	t_job	*jobs = jc_get()->active_jobs;
	t_proc	*procs;

	sig = 0;
	ignore = 0;
	pgid = getpgid(info->si_pid);
	while (jobs)
	{
		procs = jobs->procs;
		while (procs && jobs->pgid == pgid)
		{
			if (procs->pid == info->si_pid)
			{
				waitpid(procs->pid, &procs->status, WUNTRACED);
				alterate_proc(jobs, procs);
				break ;
			}
			procs = procs->next;
		}
		jobs = jobs->next;
	}
	tcsetattr(0, TCSADRAIN, g_term->shell_term);
}

void					setup_signal_handlers(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(struct sigaction));
	action.sa_handler = &handle_sigint;
	sigaction(SIGINT, &action, NULL);
	action.sa_flags = SA_RESTART;
	action.sa_handler = &tstp;
	sigaction(SIGTSTP, &action, NULL);
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = &sigchild;
	sigaction(SIGCHLD, &action, NULL);
	//signal(SIGCHLD, SIG_DFL);
	signal(SIGWINCH, &resize);
	signal(SIGPIPE, &tstp);
}
