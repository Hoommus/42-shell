/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 18:06:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/29 18:56:33 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

#define IS_HEREDOC(t) ((t) == TOKEN_DLESS || (t) == TOKEN_DLESSDASH || IS_HW(t))
#define IS_HW(t) ((t) == TOKEN_TRILESS)

static char			*add_newline(char **hereword)
{
	char	*swap;
	size_t	len;

	len = ft_strlen(*hereword);
	swap = ft_strnew(len + 1);
	ft_strcpy(swap, *hereword);
	swap[len] = '\n';
	ft_strdel(hereword);
	*hereword = swap;
	return (swap);
}

void				run_heredocs(t_node *node)
{
	struct s_io_redirect	*rdr;
	char					*swap;

	if (node && node->node_type == NODE_COMMAND && !g_interrupt)
	{
		rdr = node->command->io_redirects - 1;
		while ((++rdr)->type != TOKEN_NOT_APPLICABLE)
			if (IS_HW(rdr->type))
				add_newline(&(rdr->what.path));
			else if (IS_HEREDOC(rdr->type))
			{
				g_term->heredoc_word = rdr->what.path;
				g_term->input_state = STATE_HEREDOC;
				swap = read_arbitrary();
				if (swap == NULL)
					return ((void)(exec_abort(0)));
				free(rdr->what.path);
				rdr->what.path = swap;
			}
	}
	if (node && node->left != NULL)
		run_heredocs(node->left);
	if (node && node->right != NULL)
		run_heredocs(node->right);
}
