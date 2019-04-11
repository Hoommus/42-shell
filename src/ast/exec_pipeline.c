/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:23 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/07 12:24:41 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

#include <assert.h>
int					exec_pipeline_terminator(const t_node *node,
	t_context *context_right)
{
	int			pp[2];
	t_context	*context_left;

	assert(node->left->node_type == NODE_COMMAND
		&& node->right->node_type == NODE_COMMAND);
	if (context_right == NULL)
	{
		context_right = context_duplicate(g_term->context_original, true);
		context_remove_fd(context_right, 0);
		context_add_fd(context_right, 0, pp[0], "pipe");
	}
	context_left = context_duplicate(g_term->context_original, true);
	pipe(pp);
	context_remove_fd(context_left, 1);
	context_add_fd(context_left, 1, pp[1], "pipe");
	exec_command(node->left, context_left);
	exec_command(node->right, context_right);
	return (0); // TODO
}

int					exec_pipeline_inner(const t_node *node,
	t_context *context_right)
{
	int			pp[2];
	t_context	*context_left;

	assert(node->right->node_type != NODE_PIPE);
	assert(node->left->node_type == NODE_PIPE);
	context_left = context_duplicate(g_term->context_original, true);
	  pipe(pp);
	  context_remove_fd(context_left, 1);
	  context_add_fd(context_left, 1, pp[1], "pipe");
	  exec_pipeline_inner(node->left, context_left);
	  exec_command(node->right, context_right);
	return (0); // TODO
}

// TODO: Make special forknrun non-blocking case
int					exec_pipeline(const t_node *node)
{
	int			pp[2];
	t_context	*context_left;
	t_context	*context_right;

	assert(node->right->node_type != NODE_PIPE);
	if (node->left->node_type == NODE_PIPE)
	{
		context_left = context_duplicate(g_term->context_original, true);
		context_right = context_duplicate(g_term->context_original, true);
		context_remove_fd(context_left, 1);
		context_remove_fd(context_right, 0);
		context_add_fd(context_left, 1, pp[1], "pipe");
		context_add_fd(context_right, 0, pp[0], "pipe");
		exec_pipeline_inner(node->left, context_left);
		exec_command(node->right, context_right);
	}
	else
		exec_pipeline_terminator(node, NULL);
	return (0); // TODO
}
