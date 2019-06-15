/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:23 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/12 18:51:37 by vtarasiu         ###   ########.fr       */
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

	if (context_right == NULL)
		context_right = context_duplicate(g_term->context_original, true);
	context_left = context_duplicate(g_term->context_original, true);
	pipe(pp);
	context_remove_ofd(context_right, 0);
	context_remove_ofd(context_left, 1);
	context_add_fd(context_right, 0, pp[0], "pipe");
	context_add_fd(context_left, 1, pp[1], "pipe");
	status =
		node->left->node_type == NODE_SUBSHELL
		? exec_subshell(node->left, context_left)
		: exec_command(node->left, context_left)
		||
		node->right->node_type == NODE_SUBSHELL
		? exec_subshell(node->right, context_right)
		: exec_command(node->right, context_right);
	if (node->left->node_type == NODE_SUBSHELL)
		context_deep_free(&context_left);
	if (node->right->node_type == NODE_SUBSHELL)
		context_deep_free(&context_right);
	return (status);
}

static int			exec_pipeline_inner(const t_node *node,
	t_context *context_right)
{
	int			pp[2];
	t_context	*context_left;
	int			status;

	if ((node->left->node_type == NODE_COMMAND || node->left->node_type == NODE_SUBSHELL) &&
		(node->right->node_type == NODE_COMMAND || node->right->node_type == NODE_SUBSHELL))
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
	if (node->right->node_type == NODE_SUBSHELL)
	{
		status = exec_subshell(node->right, context_right);
		context_deep_free(&context_right);
	}
	else
		status = exec_command(node->right, context_right);
	return (status);
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
