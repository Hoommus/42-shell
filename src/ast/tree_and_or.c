/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_and_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 18:44:30 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/26 15:06:48 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_bresult	*and_or_build(const t_state *state, struct s_result *last_build)
{
	t_node			*node;
	t_bresult		*bresult;
	t_token			*start;

	start = offset_list(state->list_offset, -last_build->consumed);
	if (start->type == TOKEN_OR_IF || start->type == TOKEN_AND_IF)
		node = ast_new_node(NULL, start->type == TOKEN_OR_IF ? NODE_OR_IF
															: NODE_AND_IF);
	else
		return (and_or_finalize(state, last_build));
	bresult = ft_memalloc(sizeof(t_bresult));
	if (last_build->backup_ast == NULL)
	{
		node->right = last_build->ast->root;
		bresult->root = node;
	}
	else
	{
		node->right = last_build->backup_ast->root;
		insert_left_recursive(bresult, last_build->ast->root, node);
		bresult->root = last_build->ast->root;
		ft_memdel((void **)&(last_build->backup_ast));
	}
	ft_memdel((void **)&(last_build->ast));
	return (bresult);
}

t_bresult	*and_or_finalize(const t_state *state, struct s_result *last_build)
{
	t_node				*leftmost;
	t_bresult			*bresult;

	if (last_build->ast->root->node_type != NODE_AND_IF &&
		last_build->ast->root->node_type != NODE_OR_IF)
		return (last_build->ast);
	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	leftmost = last_build->ast->root;
	while (leftmost->left != NULL)
		leftmost = leftmost->left;
	leftmost->left = last_build->backup_ast->root;
	bresult->root = last_build->ast->root;
	ft_memdel((void **)&(last_build->ast));
	ft_memdel((void **)&(last_build->backup_ast));
	return (bresult);
}
