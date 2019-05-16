/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:23 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/09 15:57:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"
#include "shell_job_control.h"

static int			exec_pipeline_terminator(const t_node *node,
	t_context *context_right)
{
	int			pp[2];
	t_context	*context_left;
	int			status;

	if (node->left->node_type == NODE_SUBSHELL ||
		node->right->node_type == NODE_SUBSHELL)
	{
		ft_dprintf(2, SH ": subshells inside pipes are illegal\n");
		return (127);
	}
	if (context_right == NULL)
		context_right = context_duplicate(g_term->context_original, true);
	pipe(pp);
	context_left = context_duplicate(g_term->context_original, true);
	context_remove_ofd(context_right, 0);
	context_remove_ofd(context_left, 1);
	context_add_fd(context_right, 0, pp[0], "pipe");
	context_add_fd(context_left, 1, pp[1], "pipe");
	status = exec_command(node->left, context_left) ||
		exec_command(node->right, context_right);
	return (status);
}

static int			exec_pipeline_inner(const t_node *node,
	t_context *context_right)
{
	int			pp[2];
	t_context	*context_left;

	if (node->left->node_type == NODE_SUBSHELL ||
		node->right->node_type == NODE_SUBSHELL)
	{
		ft_dprintf(2, SH ": subshells inside pipes are illegal\n");
		return (127);
	}
	if (node->left->node_type == NODE_COMMAND &&
		node->right->node_type == NODE_COMMAND)
		return (exec_pipeline_terminator(node, context_right));
	if (context_right == NULL)
		context_right = context_duplicate(g_term->context_original, true);
	pipe(pp);
	context_left = context_duplicate(g_term->context_original, true);
	context_remove_ofd(context_right, 0);
	context_remove_ofd(context_left, 1);
	context_add_fd(context_right, 0, pp[0], "pipe");
	context_add_fd(context_left, 1, pp[1], "pipe");
	exec_pipeline_inner(node->left, context_left);
	return (exec_command(node->right, context_right));
}

int					exec_pipeline(const t_node *node)
{
	int		pipeline_status;
	char	*swap;

	if (node->left->node_type == NODE_PIPE)
		exec_pipeline_inner(node, NULL);
	else
		exec_pipeline_terminator(node, NULL);
	pipeline_status = jc_execute_pipeline_queue();
	jc_destroy_queue();
	environ_push_entry(jc_get()->shell_context->environ, "?",
						(swap = ft_itoa(pipeline_status)), SCOPE_SHELL_LOCAL);
	ft_memdel((void **)&swap);
	return (pipeline_status);
}
