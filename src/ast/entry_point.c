/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry_point.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:56:01 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/15 15:57:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

void			run_script(t_token *list_head, bool log_recursion)
{
	t_state				state;
	struct s_result		result;

	if (list_head == NULL)
		return ;
	ft_bzero(&state, sizeof(t_state));
	state.list_head = list_head;
	state.list_offset = list_head;
	state.rule = &g_complete_command;
	state.depth = -2;
	state.logging = log_recursion;
	result = is_syntax_valid(state);
	if (result.error != NULL)
		ft_dprintf(2, "Syntax error: %s\n", result.error->text);
	state.list_offset = offset_list(state.list_offset, result.consumed + result.valid + 1);
	ft_printf("is_valid = %s\n", result.valid ? "true" : "false");
	ft_printf("consumed de-facto: %d\n", result.consumed);
	if (state.list_offset != NULL)
		ft_dprintf(2, "21sh: \x1b[31msyntax error\x1b[0m near token '%s'\n",
			ft_strcmp(state.list_offset->value, "\n") == 0 ? "\\n" :
			state.list_offset->value);
	else
		ft_dprintf(2, "\x1b[32mSyntax valid\x1b[0m\n");
//  TODO: execute(tree);
//  TODO: free(tree);
//  TODO: free(list_head);
}
