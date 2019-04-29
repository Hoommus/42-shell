/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_auxillary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 18:12:02 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 18:56:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

int			tree_get_depth(t_node *parent)
{
	int		i;
	int		left;
	int		right;

	i = 0;
	if (parent && (parent->left != NULL || parent->right != NULL))
	{
		left = tree_get_depth(parent->left);
		right = tree_get_depth(parent->right);
		i += left > right ? left : right;
	}
	if (parent && parent->left == NULL && parent->right == NULL)
		i = 1;
	return (i);
}

t_bresult	*pipe_andor_finalize_right(const t_state *state,
										struct s_result *last_build)
{
	t_node				*tmp;

	if (state)
	{
		tmp = last_build->ast->root->right;
		ft_memdel((void **)&(last_build->ast->root));
		last_build->ast->root = tmp;
	}
	return (last_build->ast);
}
