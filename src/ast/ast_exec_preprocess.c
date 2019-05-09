/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 18:06:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/05 16:21:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

#define IS_HEREDOC(t) ((t) == TOKEN_DLESS || (t) == TOKEN_DLESSDASH)
#define IS_HEREWORD(t) ((t) == TOKEN_TRILESS)

void				run_heredocs(t_node *node)
{
	struct s_io_redirect	*rdr;
	char					*swap;

	if (node && node->node_type == NODE_COMMAND && !g_interrupt)
	{
		rdr = node->command->io_redirects - 1;
		while ((++rdr)->type != TOKEN_NOT_APPLICABLE)
			if (!IS_HEREWORD(rdr->type) && IS_HEREDOC(rdr->type)
				&& g_term->input_state != STATE_NON_INTERACTIVE)
			{
				g_term->heredoc_word = rdr->right.path;
				g_term->input_state = STATE_HEREDOC;
				swap = read_arbitrary();
				if (swap == NULL)
					return ((void)(exec_abort(0)));
				free(rdr->right.path);
				rdr->right.path = swap;
			}
	}
	if (node && node->left != NULL)
		run_heredocs(node->left);
	if (node && node->right != NULL)
		run_heredocs(node->right);
}
