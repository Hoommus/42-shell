/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/17 17:06:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"
#include "shell_history.h"
#include "shell_job_control.h"

static void				tstp(int sig)
{
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	buff_clear(0);
	ft_printf("Received SIGTSTP (%d)\n", sig);
	write(0, "\4", 1);
}

void					ignore(int sig)
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

extern void sigpipe_kill_left(t_job *pivot);

static void					sigchild_alt(__unused int sig, siginfo_t *info,
	__unused void *smthng)
{
	t_job	*list;
	int		status;

	list = jc_get()->job_queue;
	while (list->next) // never kill last process and claim its status in queue execution
	{
		if (list->pid == info->si_pid &&
			(WIFEXITED(info->si_status) || WIFSIGNALED(info->si_status)))
		{
			ft_printf("Waiting for [%d] %s\n", list->pid, list->args[0]);
			//waitpid(list->pid, &status, 0);
			list->exit_status = WEXITSTATUS(status);
			list->state = JOB_TERMINATED;
			sigpipe_kill_left(list);
			break ;
		}
		list = list->next;
	}
	sig = 0;
}

void					setup_signal_handlers(void)
{
	struct sigaction	action;
	sigset_t			mask;

 	ft_bzero(&action, sizeof(struct sigaction));
	action.__sigaction_u.__sa_handler = &tstp;
	sigaction(SIGTSTP, &action, NULL);
	action.__sigaction_u.__sa_handler = &handle_sigint;
	sigaction(SIGINT, &action, NULL);
	action.sa_flags = SA_RESTART;
	action.__sigaction_u.__sa_sigaction = &sigchild_alt;
	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	action.sa_mask = mask;
	sigaction(SIGCHLD, &action, NULL);
	signal(SIGWINCH, &resize);
}
