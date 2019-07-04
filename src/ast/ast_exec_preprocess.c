/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec_preprocess.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 18:06:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/04 19:20:48 by vtarasiu         ###   ########.fr       */
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

static bool			is_delim(enum e_token_type type)
{
	return (type != TOKEN_WORD && type != TOKEN_NAME && type != TOKEN_DLESS &&
			type != TOKEN_DLESSDASH && type != TOKEN_DLESS &&
			type != TOKEN_DGREAT && type != TOKEN_LESSAND &&
			type != TOKEN_GREATAND && type != TOKEN_LESSGREAT &&
			type != TOKEN_CLOBBER && type != TOKEN_LESS &&
			type != TOKEN_GREAT);
}

static bool			is_cmd_name(const t_token *token)
{
	const t_token	*prev = token->prev;
	const t_token	*next = token->next;

	if (token->type != TOKEN_WORD)
		return (false);
	if (prev == NULL)
		return (true);
	if (!(is_delim(prev->type) || prev->type == TOKEN_ASSIGNMENT_WORD))
		return (false);
	if (next == NULL)
		return (true);
	if ((is_delim(next->type) || (!is_string_numeric(token->value, 10) &&
		next->type >= TOKEN_TRILESS && next->type <= TOKEN_GREAT)))
		return (true);
	return (false);
}

static t_token		*insert_list_instead(t_token *insertion, t_token *instead)
{
	t_token		*lcopy;
	t_token		*nl;
	t_token		*prev = instead->prev;
	t_token		*next = instead->next;

	lcopy = insertion;
	while (lcopy->next)
		lcopy = lcopy->next;
	nl = lcopy;
	lcopy = nl->prev;
	lcopy->next = next;
	next->prev = lcopy;
	if (prev != NULL)
		prev->next = lcopy;
	free_token(nl);
	free_token(instead);
	return (insertion);
}

t_token				*apply_aliases(t_token *list_head)
{
	t_token		*copy;
	t_token		*alias;

	copy = list_head;
	while (copy)
	{
		if (is_cmd_name(copy) && alias_get(copy->value))
		{
			alias = tokenize(alias_get(copy->value), TOKEN_DELIMITERS);
			if (copy == list_head)
			{
				copy = insert_list_instead(alias, copy);
				list_head = copy;
			}
			else
				copy = insert_list_instead(alias, copy);
		}
		copy = copy->next;
	}
	return (list_head);
}
