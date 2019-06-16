/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/15 21:33:04 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
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

static struct s_result	*handle_terminal(struct s_result *result,
	const t_state *state)
{
	if (state->rule->token == TOKEN_EMPTY)
	{
		result->valid = true;
		result->consumed = 0;
	}
	else if (state->list_offset->type == state->rule->token)
	{
		result->valid = true;
		result->consumed = 1;
	}
	return (result);
}

static bool				check_rule(struct s_result *result, t_state *state,
	const t_rule *restrict const rule)
{
	struct s_result tmp;

	state->rule = rule;
	tmp = is_syntax_valid(*state);
	result->valid = tmp.valid;
	result->consumed += tmp.consumed;
	if (tmp.ast != NULL && result->ast != NULL)
		result->backup_ast = result->ast;
	if (tmp.ast != NULL)
		result->ast = tmp.ast;
	if (tmp.valid)
		state->list_offset = offset_list(state->list_offset, tmp.consumed);
	else if (tmp.valid == false && result->consumed != 0)
	{
		state->list_offset = offset_list(state->list_offset, -result->consumed);
		result->consumed = 0;
		if (result->ast)
		{
			ast_free_recursive(result->ast->root);
			ft_memdel((void **)&(result->ast));
		}
	}
	return (tmp.valid);
}

struct s_result			is_syntax_valid(t_state const prev)
{
	struct s_result		result;
	t_state				state;
	short				i;
	short				j;

	state = prev;
	ft_bzero(&result, sizeof(struct s_result));
	if (state.list_offset != NULL && IS_TERMINAL(state.rule))
		return (*handle_terminal(&result, &state));
	if (result.error != NULL)
		return (result);
	i = -1;
	while (!result.valid && ++i < 10 && prev.rule->expands_to[i][0] && (j = -1))
		while (prev.rule->expands_to[i][++j])
			if (!check_rule(&result, &state, prev.rule->expands_to[i][j]))
				break ;
	if (state.rule->tree_builder && result.valid)
		result.ast = state.rule->tree_builder(&state, &result);
	return (result);
}
