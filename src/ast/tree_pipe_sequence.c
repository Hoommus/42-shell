/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_pipe_sequence.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 13:13:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/15 17:02:58 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_bresult	*pipe_sequence_build(const t_state *state,
									struct s_result *last_build)
{
	t_node				*node;
	t_bresult			*bresult;

	if (state->rule != &g_pipe_sequence_dash)
		return (last_build->ast);
	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	node = ast_new_node(NULL, NODE_PIPE);
	if (last_build->backup_ast == NULL)
	{
		node->right = last_build->ast->root;
		bresult->root = node;
		ft_memdel((void **)&(last_build->ast));
	}
	else
	{
		node->right = last_build->backup_ast->root;
		insert_left_recursive(bresult, last_build->ast->root, node);
		bresult->root = last_build->ast->root;
		ft_memdel((void **)&(last_build->ast));
		ft_memdel((void **)&(last_build->backup_ast));
	}
	return (bresult);
}

t_bresult	*pipe_sequence_finalize(const t_state *state,
									struct s_result *last_build)
{
	t_node				*tmp;
	t_node				*pre_leftmost;
	t_node				*leftmost;
	t_bresult			*bresult;

	if (last_build->ast->root->left == NULL)
		return (pipe_andor_finalize_right(state, last_build));
	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	leftmost = last_build->ast->root->left;
	pre_leftmost = last_build->ast->root;
	while (leftmost->left != NULL)
	{
		leftmost = leftmost->left;
		pre_leftmost = pre_leftmost->left;
	}
	tmp = leftmost->right;
	bresult->root = last_build->ast->root;
	pre_leftmost->left = tmp;
	ft_memdel((void **)&(leftmost));
	ft_memdel((void **)&(last_build->ast));
	ft_memdel((void **)&(last_build->backup_ast));
	return (bresult);
}
