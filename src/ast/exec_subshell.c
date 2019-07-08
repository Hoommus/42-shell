/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 16:39:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/07 19:09:18 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

#define NINT noreturn int

static NINT	exec_subshell_async(const t_node *body, t_context *new_context)
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
	context_switch(new_context);
	close_redundant_fds(new_context);
	status = exec_node(body, NULL, false);
	exit(status);
}

static int	exec_subshell_regular(const t_node *body, t_context *new_context)
{
	int		status;
	bool	did_lock;

	jc_enable_subshell();
	did_lock = jc_lock_subshell_state(getpgid(getpid()));
	context_switch(new_context);
	status = exec_node(body, NULL, false);
	context_switch(jc_get()->shell_context);
	if (did_lock)
		jc_unlock_subshell_state(getpgid(getpid()));
	jc_disable_subshell();
	return (status);
}

static int	run_parent(int f, t_job *job, t_context *context, bool is_async)
{
	int		status;

	setpgid((job->pgid = f), f);
	sigchild_block();
	job->procs = process_create(NULL, context);
	job->procs->pid = f;
	job->command = ft_strdup("(subshell)");
	if (is_async)
		jc_register_job(job);
	tcsetpgrp(0, job->pgid);
	status = is_async ? 0 : jc_wait(job);
	tcsetpgrp(0, g_term->shell_pgid);
	sigchild_unblock();
	return (status);
}

static int	decide_what_to_do(int f, t_job *job, const t_node *body,
	t_context *context)
{
	int		status;

	status = 0;
	if (f == 0)
		exec_subshell_async(body, context);
	else if (f == -1)
		status = (ft_dprintf(2, SH ": fork error in async subshell\n") & 0) | 1;
	else if (f <= -2)
	{
		status = exec_subshell_regular(body, context);
		jc_job_dealloc(&job);
	}
	return (status);
}

int			exec_subshell(const t_node *node,
	t_context *new_context,
	bool is_async)
{
	t_context		*context;
	int				status;
	t_job			*job;
	int				f;
	const t_node	*body = node->left;

	status = 0;
	context = new_context ? new_context
			: context_duplicate(g_term->context_original, XDUP_TERM);
	f = -2;
	job = ft_memalloc(sizeof(t_job));
	jc_enable_subshell();
	jc_lock_subshell_state(g_term->shell_pgid);
	if (is_async || !(body->node_type == NODE_SEPARATOR && body->right &&
				body->right->node_type == NODE_COMMAND && body->left == NULL))
		f = fork();
	if (f <= 0)
		decide_what_to_do(f, job, body, context);
	else
		run_parent(f, job, context, is_async);
	jc_unlock_subshell_state(g_term->shell_pgid);
	jc_disable_subshell();
	return (status);
}
