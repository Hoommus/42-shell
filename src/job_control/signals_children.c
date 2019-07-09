/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_children.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 04:06:43 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 14:59:01 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"

static void		sigchild_handler(int sig, siginfo_t *info, void *ignore)
{
	pid_t		pgid;
	const t_job	*jobs = jc_get()->active_jobs;
	t_proc		*procs;

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
				if (alterate_proc((t_job *)jobs, procs) >= 2)
					break ;
			}
			procs = procs->next;
		}
		jobs = jobs->next;
	}
	tcsetattr(0, TCSADRAIN, g_term->shell_term);
}

void			sigchild_set_handler(void)
{
	static sigset_t			set;
	static struct sigaction	act;

	if (act.sa_handler == 0)
	{
		act.sa_mask = 0;
		act.sa_flags = SA_SIGINFO | SA_RESTART;
		act.sa_sigaction = &sigchild_handler;
	}
	if (!sigismember(&set, SIGCHLD))
		sigaddset(&set, SIGCHLD);
	act.sa_mask = set;
	sigaction(SIGCHLD, &act, NULL);
}

void			unset_signal_handlers(void)
{
	const static struct sigaction	act;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGWINCH, &act, NULL);
}
