/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nodes_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:56 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/04 18:55:08 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_node	*ast_new_node(void *value, enum e_node_type node_type)
{
	t_node	*node;

	node = ft_memalloc(sizeof(t_node));
	node->node_type = node_type;
	node->command = value;
	return (node);
}

void	ast_free_command_node_internals(t_node *node)
{
	struct s_command	*cmd;
	int					i;

	cmd = node->command;
	free_array((void **)cmd->args);
	free_array((void **)cmd->assignments);
	i = 0;
	while (cmd->io_redirects[i])
	{
		if ((u_int64_t)cmd->io_redirects[i]->what.fd > 10)
			ft_memdel((void **)&(cmd->io_redirects[i]->what.string));
		if ((u_int64_t)cmd->io_redirects[i]->where.fd > 10)
			ft_memdel((void **)&(cmd->io_redirects[i]->where.string));
		ft_memdel((void **)&(cmd->io_redirects[i++]));
	}
	ft_memdel((void **)&(cmd->io_redirects));
	ft_memdel((void **)&(cmd));
}

void	ast_free_recursive(t_node *node)
{
	if (node->left != NULL)
		ast_free_recursive(node->left);
	if (node->right != NULL)
		ast_free_recursive(node->right);
	if (node->node_type == NODE_COMMAND)
		ast_free_command_node_internals(node);
	else
		ft_memdel((void **)&(node->command));
	ft_memdel((void **)&node);
}
