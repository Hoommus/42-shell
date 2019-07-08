/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_if.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 16:58:13 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/08 18:41:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_script.h"

#define NINT noreturn int

static NINT	exec_and_if_async(const t_node *body)
{
	int		status;
	pid_t	pgid;

	setpgid((pgid = getpid()), getpid());
	g_term->shell_pgid = pgid;
	g_term->input_state = STATE_NON_INTERACTIVE;
	sigprocmask(SIG_SETMASK, &((sigset_t){0}), NULL);
	unset_signal_handlers();
	close(g_term->history_file);
	tcsetpgrp(0, getpgid(getpid()));
	status = exec_node(body, NULL, false);
	exit(status);
}

static int	exec_and_if_regular(const t_node *body)
{
	int		status_left;
	int		status_right;

	status_right = 1;
	status_left = exec_node(body->left, NULL, false);
	if (status_left == 0)
		status_right = exec_node(body->right, NULL, false);
	return (status_right);
}

static int	run_parent(int f, t_job *job, bool is_async)
{
	int		status;

	setpgid((job->pgid = f), f);
	sigchild_block();
	job->procs = process_create(NULL, NULL);
	job->procs->pid = f;
	job->command = ft_strdup("async &&");
	if (is_async)
		jc_register_job(job);
	tcsetpgrp(0, job->pgid);
	status = is_async ? 0 : jc_wait(job);
	tcsetpgrp(0, g_term->shell_pgid);
	sigchild_unblock();
	return (status);
}

static int	decide_what_to_do(int f, const t_node *body)
{
	int		status;

	status = 0;
	if (f == 0)
		exec_and_if_async(body);
	else if (f == -1)
		status = (ft_dprintf(2, SH ": fork error in async &&\n") & 0) | 1;
	return (status);
}

int			exec_and_if(const t_node *node,
						bool is_async)
{
	int				status;
	t_job			*job;
	int				f;

	f = -2;
	job = ft_memalloc(sizeof(t_job));
	jc_enable_subshell();
	jc_lock_subshell_state(g_term->shell_pgid);
	if (is_async)
		f = fork();
	if (f == -2)
		status = exec_and_if_regular(node);
	else if (f <= 0)
		status = decide_what_to_do(f, node);
	else
		status = run_parent(f, job, is_async);
	jc_unlock_subshell_state(g_term->shell_pgid);
	jc_disable_subshell();
	return (status);
}
