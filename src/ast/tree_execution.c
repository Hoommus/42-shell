/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 13:23:10 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/06 18:45:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

const struct s_executer		g_executers_table[NODE_COMMAND + 1] = {
	{NODE_PIPE, NULL},
	{NODE_SUBSHELL, NULL},
	{NODE_SEPARATOR, &exec_semicolon_recursive},
	{NODE_OR_IF, NULL},
	{NODE_AND_IF, NULL},
	{NODE_COMMAND, &exec_command},

	{0, NULL}
};

int			exec_node(t_node *node)
{

}

int			exec_semicolon_recursive(t_node *parent)
{
	int		status;

	status = exec_node(parent);
	if (parent->left)
		exec_semicolon_recursive(parent->left);
	if (parent->right)
		exec_semicolon_recursive(parent->right);
	return (0);
}

int			exec_semicolon_iterative(t_node *parent)
{
	int		i;
	t_node	*sep;
	t_node	*tmp;

	if (!parent || parent->node_type != NODE_SEPARATOR)
		return (ft_printf("Some weird error in tree"));
	sep = parent;
	while (sep->node_type == NODE_SEPARATOR
		&& sep->left && sep->left->node_type == NODE_COMMAND
		&& sep->right && sep->right->node_type == NODE_SEPARATOR)
	{
		exec_command(sep->left);
		tmp = sep;
		sep = sep->right;
		ast_free_recursive(tmp->left);
		ft_memdel((void **)&tmp);
	}
	return (exec_semicolon_recursive(sep));
}
