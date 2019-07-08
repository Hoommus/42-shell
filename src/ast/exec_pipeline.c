/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:23 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/21 18:33:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"
#include "shell_job_control.h"

static int			exec_pipeline_terminator(const t_node *node,
	t_context *context_right, bool is_async)
{
	int			pp[2];
	t_context	*context_left;
	int			status;

	if (context_right == NULL)
		context_right = context_duplicate(g_term->context_original, XDUP_TERM);
	context_left = context_duplicate(g_term->context_original, XDUP_TERM);
	pipe(pp);
	context_remove_ofd(context_right, 0);
	context_remove_ofd(context_left, 1);
	context_add_fd(context_right, 0, pp[0], "pipe");
	context_add_fd(context_left, 1, pp[1], "pipe");
	status = exec_node(node->left, context_left, is_async);
	status = exec_node(node->right, context_right, is_async);
	if (node->left->node_type == NODE_SUBSHELL ||
		node->left->node_type == NODE_BRACE_GROUP)
		context_deep_free(&context_left);
	if (node->right->node_type == NODE_SUBSHELL ||
		node->right->node_type == NODE_BRACE_GROUP)
		context_deep_free(&context_right);
	return (status);
}

static int			exec_pipeline_inner(const t_node *node,
	t_context *context_right, bool is_async)
{
	int			pp[2];
	t_context	*context_left;
	int			status;

	if ((node->left->node_type == NODE_COMMAND ||
		node->left->node_type == NODE_SUBSHELL) &&
		(node->right->node_type == NODE_COMMAND ||
		node->right->node_type == NODE_SUBSHELL))
		return (exec_pipeline_terminator(node, context_right, is_async));
	if (context_right == NULL)
		context_right = context_duplicate(g_term->context_original, XDUP_TERM);
	pipe(pp);
	context_left = context_duplicate(g_term->context_original, XDUP_TERM);
	context_remove_ofd(context_right, 0);
	context_remove_ofd(context_left, 1);
	context_add_fd(context_right, 0, pp[0], "pipe");
	context_add_fd(context_left, 1, pp[1], "pipe");
	exec_pipeline_inner(node->left, context_left, is_async);
	status = exec_node(node->right, context_right, is_async);
	return (status);
}

int					exec_pipeline(const t_node *node, bool is_async)
{
	int		pipeline_status;
	char	*swap;

	swap = NULL;
	if (node->left->node_type == NODE_PIPE)
		exec_pipeline_inner(node, NULL, is_async);
	else
		exec_pipeline_terminator(node, NULL, is_async);
	pipeline_status = jc_tmp_finalize(is_async);
	if (!is_async)
		environ_push_entry(jc_get()->shell_context->environ, "?",
			(swap = ft_itoa(pipeline_status)), SCOPE_SHELL_LOCAL);
	free(swap);
	return (pipeline_status);
}
