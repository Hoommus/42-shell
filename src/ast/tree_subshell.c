/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 18:08:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/15 17:45:48 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_bresult	*subshell_build(const t_state *state, struct s_result *last_build)
{
	t_node				*node;
	t_bresult			*bresult;

	bresult = ft_memalloc(sizeof(t_bresult));
	bresult->request = state->rule;
	node = ast_new_node(NULL, NODE_SUBSHELL);
	node->left = last_build->ast->root;
	bresult->root = node;
	ft_memdel((void **)&(last_build->ast));
	return (bresult);
}
