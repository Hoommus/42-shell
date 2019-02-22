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
#include "shell_script_builders.h"

t_bresult			pipeline_build(t_state *state, size_t length)
{
	t_bresult	result;
	t_node		*root;
	t_node		*tmp;
	t_token		*token_list;
	u_int32_t	i;

	i = 0;
	ft_bzero(&result, sizeof(result));
	root = ast_new_node("|", TOKEN_PIPE, NODE_GENERIC);

	return (result);
}

int					pipeline_execute(t_node *root)
{
	if (root->token_type != TOKEN_PIPE)
	{
		ft_printf("Ты втираешь мне какую-то дичь.\n");
		return (-1);
	}
	return (0);
}