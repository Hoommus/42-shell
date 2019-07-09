/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_brace_group.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/08 19:22:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/08 19:22:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_bresult	*brace_group_build(const t_state *state,
	struct s_result *last_build)
{
	t_node				*node;
	t_bresult			*bresult;

	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	node = ast_new_node(NULL, NODE_BRACE_GROUP);
	node->left = last_build->ast->root;
	bresult->root = node;
	ft_memdel((void **)&(last_build->ast));
	return (bresult);
}
