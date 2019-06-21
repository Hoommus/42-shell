/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 13:23:10 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/20 20:02:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_script.h"

volatile sig_atomic_t		g_interrupt;

const struct s_executor		g_executors_table[] = {
	{ NODE_COMMAND, {.exec_with_context = &exec_command} },
	{ NODE_SEPARATOR, {&exec_semicolon_recursive} },
	{ NODE_PIPE, {&exec_pipeline} },
	{ NODE_OR_IF, {&exec_or_if} },
	{ NODE_AND_IF, {&exec_and_if} },
	{ NODE_SUBSHELL, {.exec_with_context = &exec_subshell} },
	{ 0, {NULL}}
};

int							exec_abort(int dummy)
{
	dummy = 0;
	return (g_interrupt = -77);
}

int exec_node(const t_node *node, t_context *context, bool is_async)
{
	int		i;

	if (node->lasync)
		ft_printf("Executing async node\n");
	i = -1;
	while (g_executors_table[++i].executor.exec && !g_interrupt)
		if (node->node_type == g_executors_table[i].node_type)
			return (g_executors_table[i].executor.exec_with_context(node, context, is_async));
	if (!g_interrupt)
		ft_printf("No executor for node of type %d\n", node->node_type);
	return (71);
}

int exec_semicolon_recursive(const t_node *parent, bool is_async)
{
	int		status;

	status = is_async & 0;
	if (parent->left)
	{
		ft_printf("Executing node of type %d %s\n", parent->left->node_type,
			parent->lasync ? "asynchronously" : "");
		status = exec_node(parent->left, NULL, parent->lasync);
		ft_printf("Calling finalization from semicolon node\n");
		//jc_tmp_finalize(parent->lasync);
	}
	if (parent->right)
		status = exec_node(parent->right, NULL, parent->rasync);
	g_term->last_status = status;
	return (status);
}

/*
** Entry point in any AST
**
** This function exists solely for handling one specific yet very common AST
** structure. It just iterates to some point and then passes control to classic
** recursive execution.
**
** Because tree balancing is for virgins and crutches are for real chads
*/

int							exec_semicolon_iterative(t_node *parent)
{
	t_node	*sep;

	g_interrupt = 0;
	if (!parent)
		return (ft_dprintf(2, SH ": AST root is null.\n"));
	if (parent->node_type != NODE_SEPARATOR)
		exec_node(parent, NULL, false);
	sep = parent;
	while (sep->node_type == NODE_SEPARATOR
		&& sep->left && sep->left->node_type == NODE_COMMAND
		&& sep->right && sep->right->node_type == NODE_SEPARATOR
		&& g_interrupt == 0)
	{
		exec_command(sep->left, NULL, sep->lasync);
		sep = sep->right;
	}
	return (exec_semicolon_recursive(sep, false));
}
