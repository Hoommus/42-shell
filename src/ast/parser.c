/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:12:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/25 17:33:25 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script_parser.h"
#include "shell_script_syntax.h"
#include "shell_script.h"

#define INDENT "│ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ │ "

t_state			*offset_destructive(t_state *state, int offset)
{
	int		i;

	i = 0;
	while (i < offset && state->list_head)
	{
		state->list_head = state->list_head->next;
		i++;
	}
	return (state);
}

t_token			*list_offset(t_token *list, int offset)
{
	int		i;

	i = -1;
	while (offset < 0 && ++i < -offset && list)
		list = list->prev;
	while (offset > 0 && ++i < offset && list)
		list = list->next;
	return (list);
}

t_token			*list_revert(t_token *list, int offset)
{
	return (list_offset(list, -offset));
}

struct s_result	*handle_terminal(struct s_result *result, t_state *state)
{
	ft_printf("%.*s  %s terminal in rule [%s] at %s (%s) \n",
		2 + (state->depth) * 2, INDENT, "└─",
		state->rule->human_readable,
		g_tokens[state->list_offset->type].token_name,
		state->list_offset->value);
	if (state->rule->token == TOKEN_EMPTY)
	{
		ft_printf("%.*s    %s empty is allowed\n", 2 + (state->depth) * 2, INDENT, "└─");
		result->valid = true;
		result->to_skip = 0;
	}
	else
	{
		ft_printf("%.*s    %s expected token: (%s); got (%s)\n",
			2 + (state->depth) * 2, INDENT, "└─",
			g_tokens[state->rule->token].token_name,
			g_tokens[state->list_offset->type].token_name);
		result->valid = state->list_offset->type == state->rule->token;
		result->to_skip = result->valid;
	}
	return (result);
}

struct s_result	is_syntax_valid(t_state const prev_state)
{
	t_state				state;
	struct s_result		result;
	struct s_result		tmp;
	short				i;
	short				j;

	state = prev_state;
	ft_bzero(&result, sizeof(struct s_result));
	if (state.rule != NULL && state.list_offset == NULL)
	{
		ft_printf("Unexpected end of command at: %s\n", state.list_offset->prev->value);
		return (result);
	}
	else if (IS_TERMINAL(state.rule))
		return (*handle_terminal(&result, &state));
	ft_printf("%.*s%s inside [%s]; head token [%s, %s]\n", 2 + (++state.depth) * 2, INDENT, "└─",
		state.rule->human_readable,
		g_tokens[state.list_offset->type].token_name,
		state.list_offset->value);
	i = -1;
	while (!result.valid && ++i < 10 && prev_state.rule->expands_to[i][0] && (j = -1))
	{
		while (prev_state.rule->expands_to[i][++j])
		{
			state.rule = prev_state.rule->expands_to[i][j];
			tmp = is_syntax_valid(state);
			result.valid = tmp.valid;
			result.to_skip += tmp.to_skip;
			if (tmp.valid == true)
				state.list_offset = list_offset(state.list_offset, result.to_skip);
			else
			{
				state.list_offset = list_revert(state.list_offset, result.to_skip);
				break ;
			}
		}
	}
	return (result);
}

void		run_script(t_token *stream_head)
{
	t_state				initial_state;
	struct s_result		result;

	ft_bzero(&initial_state, sizeof(t_state));
	if (stream_head == NULL)
		return ;
	initial_state.list_head = stream_head;
	initial_state.list_offset = stream_head;
	initial_state.rule = &g_program;
	initial_state.depth = 0;
	result = is_syntax_valid(initial_state);
	if(result.valid)
		ft_printf("Syntax valid\n");
	else
		ft_printf("Syntax invalid\n");
//  TODO: free(stream_head);
}
