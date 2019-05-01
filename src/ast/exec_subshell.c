/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 16:39:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/01 14:29:41 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"
#include "shell_job_control.h"

#include <assert.h>
int					subshell_optimised(const t_node *parent)
{
	t_context	*context;
	int			wexitstatus;

	context = context_duplicate(g_term->context_original, true);
	context_switch(context);
	wexitstatus = exec_node((t_node *)parent);
	context_deep_free(&context);
	context_switch(jc_get()->shell_context);
	return (wexitstatus);
}

// TODO: consider removing fork
int					subshell_forknrun(const t_node *parent)
{
	t_context	*context;
	int			status;
	int			wexitstatus;
	pid_t		pid;

	context = context_duplicate(jc_get()->shell_context, true);
	if ((pid = fork()) == 0)
	{
		context_switch(context);
		exit(exec_semicolon_iterative((t_node *)parent));
	}
	else if (pid == -1)
		ft_dprintf(2, SH ": fork error in subshell.\n");
	else
	{
		context_deep_free(&context);
		while ((waitpid(pid, &status, 0)) == -1)
			;
		wexitstatus = WEXITSTATUS(status);
		return (wexitstatus);
	}
	return (-256);
}

int					exec_subshell(const t_node *node)
{
	assert(node->node_type == NODE_SUBSHELL);
	assert(node->right == NULL);
	return (subshell_optimised(node->left));
//	return (subshell_forknrun(node->left));
}
