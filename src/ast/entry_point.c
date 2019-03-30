/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:01 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/20 15:59:43 by vtarasiu         ###   ########.fr       */
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

void			run_script(t_token *list_head, bool log_recursion)
{
	t_state				state;
	struct s_result		result;

	if (list_head == NULL)
		return ;
	init_state(&state, list_head, log_recursion);
	result = is_syntax_valid(state);
	if (result.error != NULL)
		ft_dprintf(2, "Syntax error: %s\n", result.error->text);
	state.list_offset = offset_list(state.list_offset, result.consumed + result.valid);
	if (state.list_offset != NULL)
		ft_dprintf(2,
			"21sh: syntax error near token '%s' on line %d\n",
			ft_strcmp(state.list_offset->value, "\n") == 0 ? "\\n" :
			state.list_offset->value,
			state.list_offset->line_nbr);
	// TODO: solve heredocs problem
	if (result.ast && state.list_offset == NULL)
		exec_semicolon_iterative(result.ast->root);
	free_all(&state, &result);
}
