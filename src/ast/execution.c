/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 13:23:10 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 16:30:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

volatile sig_atomic_t		g_interrupt;

const struct s_executor		g_executors_table[] = {
	{ NODE_COMMAND, {.exec_alt_context = &exec_command} },
	{ NODE_SEPARATOR, {&exec_semicolon_recursive} },
	{ NODE_PIPE, {&exec_pipeline} },
	{ NODE_OR_IF, {&exec_or_if} },
	{ NODE_AND_IF, {&exec_and_if} },
	{ NODE_SUBSHELL, {NULL} },
	{ 0, {NULL }}
};

int							exec_abort(int dummy)
{
	dummy = 0;
	return (g_interrupt = -77);
}

int							exec_node(const t_node *node)
{
	int		i;

	i = -1;
	while (g_executors_table[++i].executor.exec && !g_interrupt)
		if (node->node_type == g_executors_table[i].node_type)
			return (g_executors_table[i].executor.exec_alt_context(node, NULL));
	if (!g_interrupt)
		ft_printf("No executor for node of type %d\n", node->node_type);
	return (71);
}

int							exec_semicolon_recursive(const t_node *parent)
{
	int		status;

	if (parent->left)
		status = exec_node(parent->left);
	if (parent->right)
		status = exec_node(parent->right);
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
	if (!parent || parent->node_type != NODE_SEPARATOR)
		return (ft_dprintf(2, "21sh: Some weird error in AST\n"));
	sep = parent;
	while (sep->node_type == NODE_SEPARATOR
		&& sep->left && sep->left->node_type == NODE_COMMAND
		&& sep->right && sep->right->node_type == NODE_SEPARATOR
		&& g_interrupt == 0)
	{
		exec_command(sep->left, NULL);
		sep = sep->right;
	}
	return (exec_semicolon_recursive(sep));
}
