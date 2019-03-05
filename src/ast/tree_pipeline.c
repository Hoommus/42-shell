/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/15 15:56:04 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

t_bresult			pipeline_build(t_state *state, size_t length)
{
	t_bresult	result;
	t_node		*root;
	t_node		*tmp;
	t_token		*token_list;
	u_int32_t	i;

	i = 0;
	ft_bzero(&result, sizeof(result));
	root = ast_new_node("|", NODE_GENERIC);

	return (result);
}

int					pipeline_execute(t_node *root)
{
	return (0);
}
