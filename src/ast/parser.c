/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/26 18:34:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script_parser.h"
#include "shell_script.h"

t_token					*offset_list(t_token *list, int offset)
{
	int		i;

	i = -1;
	while (offset < 0 && ++i < -offset && list)
		list = list->prev;
	while (offset > 0 && ++i < offset && list)
		list = list->next;
	return (list);
}

struct s_result	*handle_terminal(struct s_result *result, const t_state *state)
{
	if (state->logging)
		ft_printf(LOG_TERMINAL, 2 + (state->depth) * 2, INDENT, "└─",
			state->rule->human_readable,
			g_tokens[state->list_offset->type].token_name,
			state->list_offset->type == TOKEN_NEWLINE ? "\\n" :
			state->list_offset->value);
	if (state->rule->token == TOKEN_EMPTY)
	{
		if (state->logging)
			ft_printf(LOG_EMPTY, 2 + (state->depth) * 2, INDENT, "└─");
		result->valid = true;
		result->consumed = 0;
	}
	else
	{
		if (state->rule->token == state->list_offset->type && state->logging)
			ft_printf(LOG_EXPECT, 2 + (state->depth) * 2, INDENT, "└─",
				g_tokens[state->rule->token].token_name,
				state->list_offset->type == TOKEN_NEWLINE ? "\\n" :
				g_tokens[state->list_offset->type].token_name);
		if (state->list_offset->type == state->rule->token)
		{
			result->valid = true;
			result->consumed = 1;
		}
	}
	return (result);
}

bool				check_rule(struct s_result *result, t_state *state,
								const t_rule *restrict const rule)
{
	struct s_result		tmp;

	state->rule = rule;
	tmp = is_syntax_valid(*state);
	result->valid = tmp.valid;
	result->consumed += tmp.consumed;
	if (tmp.valid)
	{
		state->list_offset = offset_list(state->list_offset, tmp.consumed);
	}
	else if (tmp.valid == false && result->consumed != 0)
	{
		state->list_offset = offset_list(state->list_offset, -result->consumed);
		result->consumed = 0;
		ast_free_recursive(result->node->ast_root);
	}
	return (tmp.valid);
}

// TODO: Remove logging
struct s_result			is_syntax_valid(t_state const prev)
{
	struct s_result		result;
	t_state				state;
	short				i;
	short				j;

	state = prev;
	ft_bzero(&result, sizeof(struct s_result));
	if (state.logging)
		ft_printf(LOG_INSIDE, (state.depth += 2) * 2, INDENT, "└─",
			state.rule->human_readable);
	if (state.rule != NULL && state.list_offset == NULL)
		result.error = &unexpected_end;
	else if (IS_TERMINAL(state.rule))
		return (*handle_terminal(&result, &state));
	if (result.error != NULL)
		return (result);
	i = -1;
	while (!result.valid && ++i < 10 && prev.rule->expands_to[i][0] && (j = -1))
		while (prev.rule->expands_to[i][++j])
			if (!check_rule(&result, &state, prev.rule->expands_to[i][j]))
				break ;
	if (state.rule->tree_builder && result.valid)
		result.node = state.rule->tree_builder(&state, result.consumed);
	if (result.node)
		print_command_node(result.node->ast_root);
	//ft_printf("consumed %d tokens\n", result.consumed);
	return (result);
}
