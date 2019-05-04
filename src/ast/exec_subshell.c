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
#include "shell_job_control.h"

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

int					exec_subshell(const t_node *node)
{
	return (subshell_optimised(node->left));
}
