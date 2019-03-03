/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_pipe_sequence.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/02 13:13:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/03 16:30:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

#include <assert.h>
t_bresult	*insert_left_recursive(t_bresult *bresult,
									t_node *parent,
									t_node *insertion)
{
	if (parent->left == NULL)
		parent->left = insertion;
	else
		insert_left_recursive(bresult, parent->left, insertion);
	return (bresult);
}

t_bresult	*pipe_sequence_dash_build(const t_state *state,
									struct s_result *last_build)
{
	t_node				*node;
	t_bresult			*bresult;

	if (state->rule != &g_pipe_sequence_dash)
		return (last_build->ast);
	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	node = ast_new_node(NULL, TOKEN_PIPE, NODE_PIPE);
	if (last_build->backup_ast == NULL)
	{
		node->right = last_build->ast->ast_root;
		bresult->ast_root = node;
		ft_memdel((void **)&(last_build->ast));
	}
	else
	{
		node->right = last_build->backup_ast->ast_root;
		insert_left_recursive(bresult, last_build->ast->ast_root, node);
		bresult->ast_root = last_build->ast->ast_root;
		ft_memdel((void **)&(last_build->ast));
		ft_memdel((void **)&(last_build->backup_ast));
	}
	return (bresult);
}

t_bresult	*pipe_sequence_finalizer(const t_state *state,
									struct s_result *last_build)
{
	t_node				*tmp;
	t_node				*pre_leftmost;
	t_node				*leftmost;
	t_bresult			*bresult;

	assert(state->rule == &g_pipe_sequence);
	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	leftmost = last_build->ast->ast_root->left;
	pre_leftmost = last_build->ast->ast_root;
	while (leftmost->left != NULL)
	{
		leftmost = leftmost->left;
		pre_leftmost = pre_leftmost->left;
	}
	tmp = leftmost->right;
	bresult->ast_root = last_build->ast->ast_root;
	pre_leftmost->left = tmp;
	ft_memdel((void **)&(leftmost));
	ft_memdel((void **)&(last_build->ast));
	ft_memdel((void **)&(last_build->backup_ast));
	return (bresult);
}
