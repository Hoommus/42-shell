/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 16:39:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/18 14:55:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

bool				g_is_subshell_env;

int exec_subshell(const t_node *node, t_context *new_context, bool is_async)
{
	t_context	*context;
	int			status;
	t_job_alt	*job;
	int			f;

	if (!new_context)
		context = context_duplicate(g_term->context_original, true);
	else
		context = new_context;
	f = -2;
	if (is_async || node->left->node_type != NODE_COMMAND)
	{
		job = ft_memalloc(sizeof(t_job_alt));
		job->state = JOB_BG;
		f = fork();
	}
	g_is_subshell_env = true;
	if (f == -2 || f == 0)
	{
		if (f == 0)
			setpgid(getpid(), getpid());
		context_switch(context);
		status = exec_node((t_node *) node->left, NULL, false);
		if (!new_context)
			context_deep_free(&context);
		context_switch(jc_get()->shell_context);
		if (f == 0)
			exit(status);
		g_is_subshell_env = false;
		return (0);
	}
	else if (f == -1)
	{
		jc_job_dealloc(&job);
		return ((ft_dprintf(2, SH ": fork error in async subshell\n") & 0) | 1);
	}
	g_is_subshell_env = false;
	setpgid((job->pgid = f), f);
	jc_register_job(job);
	return (0);
}
