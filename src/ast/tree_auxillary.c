/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_auxillary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:12:02 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/27 17:15:59 by vtarasiu         ###   ########.fr       */
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
	char				*tmp;

	if (node->node_type != NODE_COMMAND)
		return ;
	cmd = (struct s_command *)node->value;
	ft_printf("COMMAND [ %s ] :\n", cmd->is_async ? "async" : "regular");
	if (cmd->args && cmd->args[0])
	{
		ft_printf("    args: [\x1b[36m %s \x1b[0m]\n", tmp = ft_strarr_join(", ", cmd->args));
		free(tmp);
	}
	if (cmd->assignments && cmd->assignments[0])
	{
		ft_printf("    assignments: [ %s ] \n", tmp = ft_strarr_join(", ", cmd->assignments));
		free(tmp);
	}
	ft_printf("    redirects: [ ");
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
