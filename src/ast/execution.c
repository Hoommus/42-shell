/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 13:23:10 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/09 14:24:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

#include <assert.h>
volatile sig_atomic_t		g_is_interrupted;

const struct s_executor		g_executors_table[] = {
	{ NODE_COMMAND,   {.exec_alt_context = &exec_command} },
	{ NODE_SEPARATOR, {&exec_semicolon_recursive} },
	{ NODE_PIPE,      {&exec_pipeline} },
	{ NODE_OR_IF,     {&exec_or_if} },
	{ NODE_AND_IF,    {&exec_and_if} },
	{ NODE_SUBSHELL,  {NULL} },
	{ 0, {NULL }}
};

int							exec_abort(int dummy)
{
	dummy = 0;
	return (g_is_interrupted = -77);
}

int							exec_node(const t_node *node)
{
	int		i;

	i = -1;
	while (g_executors_table[++i].executor.exec && !g_is_interrupted)
		if (node->node_type == g_executors_table[i].node_type)
			return (g_executors_table[i].executor.exec_alt_context(node, NULL));
	if (!g_is_interrupted)
		ft_printf("No executor for node of type %d\n", node->node_type);
	return (71);
}

int							exec_and_if(const t_node *parent)
{
	int		status_left;
	int		status_right;

	assert(parent && parent->node_type == NODE_AND_IF);
	status_right = 0;
	status_left = exec_node(parent->left);
	if (status_left == 0)
		status_right = exec_node(parent->right);
	return (status_left && status_right);
}

int							exec_or_if(const t_node *parent)
{
	int		status_left;
	int		status_right;

	assert(parent && parent->node_type == NODE_OR_IF);
	status_right = 0;
	status_left = exec_node(parent->left);
	if (status_left != 0)
		status_right = exec_node(parent->right);
	return (status_left == 0 || status_right == 0);
}

int							exec_semicolon_recursive(const t_node *parent)
{
	assert(parent && parent->node_type == NODE_SEPARATOR);
	if (parent->left)
		exec_node(parent->left);
	if (parent->right)
		exec_node(parent->right);
	return (0); // always successful
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

	g_is_interrupted = 0;
	if (!parent || parent->node_type != NODE_SEPARATOR)
		return (ft_printf("Some weird error in tree\n"));
	sep = parent;
	while (sep->node_type == NODE_SEPARATOR
		&& sep->left && sep->left->node_type == NODE_COMMAND
		&& sep->right && sep->right->node_type == NODE_SEPARATOR
		&& g_is_interrupted == 0)
	{
		exec_command(sep->left, NULL);
		sep = sep->right;
	}
	return (exec_semicolon_recursive(sep));
}
