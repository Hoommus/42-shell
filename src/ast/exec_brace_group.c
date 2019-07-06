/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_brace_group.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/06 15:51:01 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/06 15:51:01 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

#define NINT noreturn int

static NINT	exec_brace_group_async(const t_node *node,
	t_context *new_context,
	bool is_async)
{
	int		status;

	setpgid(getpid(), getpid());
	context_switch(new_context);
	status = exec_node((t_node *)node->left, NULL, is_async);
	jc_tmp_finalize(false);
	exit(status);
}

static int	exec_brace_group_regular(const t_node *node,
	t_context *new_context,
	bool is_async)
{
	int		status;

	context_switch(new_context);
	status = exec_node((t_node *)node->left, NULL, is_async);
	context_switch(jc_get()->shell_context);
	return (status);
}

int			exec_brace_group(const t_node *node,
	t_context *new_context,
	bool is_async)
{
	t_context	*context;
	int			status;
	t_job		*job;
	int			f;

	status = 0;
	if (!(context = new_context))
		context = context_duplicate(g_term->context_original, XDUP_TERM);
	f = -2;
	job = ft_memalloc(sizeof(t_job));
	if (is_async || !(node->left->node_type == NODE_SEPARATOR
						&& node->left->right && node->left->left == NULL))
		f = fork();
	if (f == 0)
		exec_brace_group_async(node, context, is_async);
	else if (f == -1)
		status = (ft_dprintf(2, SH ": fork error in async braces\n") & 0) | 1;
	else if (f == -2)
	{
		status = exec_brace_group_regular(node, context, is_async);
		jc_job_dealloc(&job);
	}
	else if (f > 0)
	{
		setpgid((job->pgid = f), f);
		job->procs = process_create(NULL, context);
		job->procs->pid = f;
		job->command = ft_strdup("{ brace group }");
		jc_register_job(job);
		if (!is_async)
			jc_wait(job);
		status = 0;
	}
	return (status);
}
