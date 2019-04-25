/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 18:06:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/23 18:06:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

// TODO: write replace_all function
char				*postprocess_dlessdash(char *input)
{
	char	*swap;
	int		i;

	while ((swap = ft_strchr(input, '\t')))
	{
		if (swap != input && *(swap - 1) != '\n')
			continue ;
		i = 0;
		while (swap[i] == '\t')
			i++;
		ft_memmove(swap, swap + i, ft_strlen(swap) - i + 1);
	}
	return (swap);
}

void				run_heredocs(t_node *node)
{
	struct s_io_redirect	*io_rdr;
	char					*swap;

	if (node && node->node_type == NODE_COMMAND && !g_interrupt)
	{
		io_rdr = node->command->io_redirects;
		while (io_rdr->type != TOKEN_NOT_APPLICABLE)
		{
			if (io_rdr->type == TOKEN_DLESS || io_rdr->type == TOKEN_DLESSDASH)
			{
				g_term->heredoc_word = io_rdr->what.path;
				g_term->input_state = STATE_HEREDOC;
				swap = read_arbitrary();
				if (swap == NULL)
					return ((void)(exec_abort(0)));
				free(io_rdr->what.path);
				io_rdr->what.path = swap;
			}
			io_rdr++;
		}
	}
	if (node && node->left != NULL)
		run_heredocs(node->left);
	if (node && node->right != NULL)
		run_heredocs(node->right);
}
