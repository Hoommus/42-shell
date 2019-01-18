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
//	ft_printf("%.*s  %s terminal in rule [%s] at %s (%s) \n",
//		2 + (state->depth) * 2, INDENT, "└─",
//		state->rule->human_readable,
//		g_tokens[state->list_offset->type].token_name,
//		state->list_offset->type == TOKEN_NEWLINE ? "\\n" :
//			state->list_offset->value);
	if (state->rule->token == TOKEN_EMPTY)
	{
//		ft_printf("%.*s    \x1b[35m%s empty allowed, validating\x1b[0m\n",
//			2 + (state->depth) * 2, INDENT, "└─");
		result->valid = true;
		result->to_skip = 0;
	}
	else
	{
//		if (state->rule->token == state->list_offset->type)
//			ft_printf("%.*s    \x1b[32m%s expected token: (%s); got (%s)\x1b[0m\n",
//				2 + (state->depth) * 2, INDENT, "└─",
//				g_tokens[state->rule->token].token_name,
//				state->list_offset->type == TOKEN_NEWLINE ? "\\n" :
//				g_tokens[state->list_offset->type].token_name);
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
	ft_bzero(&tmp, sizeof(struct s_result));
//	ft_printf("%.*s%s inside [ \x1b[36m%s\x1b[0m ];\n", (state.depth += 2) * 2, INDENT, "└─",
//		state.rule->human_readable);
	if (state.rule != NULL && state.list_offset == NULL)
	{
//		if (state.list_offset != NULL)
//			ft_printf("Unexpected end of command\n");
		result.to_skip = 2147483647;
		return (result);
	}
	else if (IS_TERMINAL(state.rule))
		return (*handle_terminal(&result, &state));
	i = -1;
	while (!result.valid &&
		++i < 10 && prev_state.rule->expands_to[i][0] && (j = -1))
	{
		//result.to_skip = 0;
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
	initial_state.depth = -2;
	result = is_syntax_valid(initial_state);
	if (result.error != NULL)
		ft_printf("Syntax error: %s\n", result.error->text);
	initial_state.list_offset = list_offset(initial_state.list_offset, result.to_skip + 1);
	if (initial_state.list_offset != NULL)
		ft_printf("List offset is not null, lol: (%s)[%s]\n",
			g_tokens[initial_state.list_offset->type].token_name,
			initial_state.list_offset->value);
	if(result.valid && initial_state.list_offset == NULL)
		ft_printf("Syntax valid\n");
	else
		ft_printf("Syntax invalid\n");
//  TODO: free(stream_head);
}
