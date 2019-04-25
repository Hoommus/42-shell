/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 14:47:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 16:26:42 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_bresult	*list_build(const t_state *state, struct s_result *last_build)
{
	t_node				*node;
	t_bresult			*bresult;

	if (state->rule != &g_list_dash)
		return (last_build->ast);
	bresult = ft_memalloc(sizeof(t_bresult));
	if (last_build->backup_ast == NULL || last_build->ast->root->left != NULL)
	{
		node = ast_new_node(NULL, NODE_SEPARATOR);
		node->right = last_build->ast->root;
		if (last_build->backup_ast != NULL)
			node->left = last_build->backup_ast->root;
		bresult->root = node;
	}
	else if (last_build->ast->root->node_type != NODE_SEPARATOR &&
			last_build->ast->root->left != NULL)
		return (bresult);
	else
	{
		last_build->ast->root->left = last_build->backup_ast->root;
		bresult->root = last_build->ast->root;
	}
	ft_memdel((void **)&(last_build->ast));
	ft_memdel((void **)&(last_build->backup_ast));
	return (bresult);
}
