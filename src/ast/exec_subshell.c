/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 16:39:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/09 13:26:12 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

bool				g_is_subshell_env;

int					exec_subshell(const t_node *node, t_context *new_context)
{
	t_context	*context;
	int			wexitstatus;

	if (!new_context)
		context = context_duplicate(g_term->context_original, true);
	else
		context = new_context;
	g_is_subshell_env = true;
	context_switch(context);
	wexitstatus = exec_node((t_node *)node->left);
	context_deep_free(&context);
	context_switch(jc_get()->shell_context);
	g_is_subshell_env = false;
	return (wexitstatus);
}
