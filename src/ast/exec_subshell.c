/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 16:39:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/27 16:54:53 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

#define NINT noreturn int

bool	g_is_subshell_env;

NINT	exec_subshell_async(const t_node *node, t_context *new_context)
{
	int		status;

	g_is_subshell_env = true;
	setpgid(getpid(), getpid());
	context_switch(new_context);
	status = exec_node((t_node *)node->left, NULL, false);
	jc_tmp_finalize(false);
	exit(status);
}

int		exec_subshell_regular(const t_node *node, t_context *new_context)
{
	int		status;

	g_is_subshell_env = true;
	context_switch(new_context);
	status = exec_node((t_node *)node->left, NULL, false);
	context_switch(jc_get()->shell_context);
	g_is_subshell_env = false;
	return (status);
}

int		exec_subshell(const t_node *node, t_context *new_context, bool is_async)
{
	t_context	*context;
	int			status;
	t_job		*job;
	int			f;

	status = 0;
	context = new_context ? new_context
			: context_duplicate(g_term->context_original, XDUP_TERM);
	f = -2;
	job = ft_memalloc(sizeof(t_job));
	if (is_async || (node->left->node_type != NODE_COMMAND)
				|| (node->left->node_type == NODE_SEPARATOR
						&& node->left->left
						&& node->left->right))
		f = fork();
	if (f == 0)
		exec_subshell_async(node, context);
	else if (f == -1)
		status = (ft_dprintf(2, SH ": fork error in async subshell\n") & 0) | 1;
	else if (f == -2)
	{
		status = exec_subshell_regular(node, context);
		jc_job_dealloc(&job);
	}
	else if (f > 0)
	{
		setpgid((job->pgid = f), f);
		job->procs = process_create(NULL, context);
		job->procs->pid = f;
		job->command = ft_strdup("(subshell)");
		jc_register_job(job);
		if (!is_async)
			jc_wait(job);
		status = 0;
	}
	return (status);
}
