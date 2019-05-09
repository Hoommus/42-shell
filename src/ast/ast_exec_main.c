/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_exec_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:01 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/09 11:52:43 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

static inline void	init_state(t_state *state, t_token *list_head, bool log)
{
	ft_bzero(state, sizeof(t_state));
	state->list_head = list_head;
	state->list_offset = list_head;
	state->rule = &g_complete_command;
	state->depth = -2;
	state->logging = log;
}

static inline void	free_all(t_state *state, struct s_result *result)
{
	t_token				*swap;

	while (state->list_head)
	{
		swap = state->list_head->next;
		free_token(state->list_head);
		state->list_head = swap;
	}
	if (result->ast && result->ast->root)
	{
		ast_free_recursive(result->ast->root);
		ft_memdel((void **)&(result->ast));
	}
	if (result->backup_ast && result->backup_ast->root)
		ast_free_recursive(result->backup_ast->root);
}

/*
** TODO: Remove that crutch with newlines checks
*/

void				run_script(t_token *list_head, bool log_recursion)
{
	t_state				state;
	struct s_result		result;

	if (list_head == NULL)
		return ;
	init_state(&state, list_head, log_recursion);
	result = is_syntax_valid(state);
	state.list_offset = offset_list(state.list_offset, result.consumed +
		result.valid);
	if (state.list_offset && state.list_offset->type == TOKEN_NEWLINE &&
		state.list_offset->prev->type == TOKEN_NEWLINE)
		state.list_offset = state.list_offset->next;
	if (state.list_offset != NULL)
		g_term->last_status = (ft_dprintf(2, ERR_SYNTAX_AT_LINE,
			ft_strcmp(state.list_offset->value, "\n") == 0 ? "\\n" :
			state.list_offset->value, state.list_offset->line_nbr) & 0) | 1;
	if (result.ast && state.list_offset == NULL)
	{
		run_heredocs(result.ast->root);
		if (!g_interrupt)
			g_term->last_status = exec_semicolon_iterative(result.ast->root);
		else
			g_term->last_status = 1;
	}
	free_all(&state, &result);
}
