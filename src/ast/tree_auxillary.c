/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_auxillary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:12:02 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/26 17:21:31 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

int			get_tree_depth(t_node *parent)
{
	int		i;
	int		left;
	int		right;


	i = 0;
	if (parent && (parent->left != NULL || parent->right != NULL))
	{
		left = get_tree_depth(parent->left);
		right = get_tree_depth(parent->right);
		i += left > right ? left : right;
	}
	if (parent && parent->left == NULL && parent->right == NULL)
		i = 1; // last layer
	return (i);
}

void		print_command_node(t_node *node)
{
	struct s_command	*cmd;

	if (node->node_type == NODE_COMMAND)
	{
		ft_printf("COMMAND ");
		cmd = (struct s_command *)node->value;
		ft_printf("[ %s ] :\n", cmd->is_bg ? "async" : "regular");
		ft_printf("args: [ ");
		for (int i = 0; cmd->args[i] != NULL; i++)
			ft_printf("%s, ", cmd->args[i]);
		ft_printf("] \n assignments: [ ");
		for (int i = 0; cmd->assignments[i] != NULL; i++)
			ft_printf("%s, ", cmd->assignments[i]);
		ft_printf("] \n redirects: [ ");
		for (int i = 0; cmd->io_redirects[i] != NULL; i++)
		{
			if ((u_int32_t)cmd->io_redirects[i]->what.fd < 10)
				ft_printf("%d ", cmd->io_redirects[i]->what.fd);
			else
				ft_printf("%s ", cmd->io_redirects[i]->what.string);
			ft_printf("%s ", g_tokens[cmd->io_redirects[i]->type].token_name);
			if ((u_int32_t)cmd->io_redirects[i]->where.fd < 10)
				ft_printf("%d ", cmd->io_redirects[i]->where.fd);
			else
				ft_printf("%s ", cmd->io_redirects[i]->where.string);
		}
		ft_printf("] \n");
	}
}
