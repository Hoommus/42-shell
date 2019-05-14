/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:57:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/10 12:37:41 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

static bool					is_token_an_arg(t_token *pivot)
{
	if (pivot->type != TOKEN_WORD)
		return (false);
	if (is_redirect(pivot->prev))
		return (false);
	if ((is_redirect(pivot->next) && is_string_numeric(pivot->value, 10)))
		return (false);
	return (true);
}

static char					**get_args(t_token *list, int length)
{
	char	**args;
	int		i;
	int		j;
	int		size;
	t_token	*copy;

	i = -1;
	size = 0;
	copy = list;
	while (copy && ++i < length)
	{
		if (is_token_an_arg(copy))
			size++;
		copy = copy->next;
	}
	args = ft_memalloc(sizeof(char *) * (size + 1));
	i = -1;
	j = 0;
	while (list && ++i < length)
	{
		if (is_token_an_arg(list))
			args[j++] = ft_strdup(list->value);
		list = list->next;
	}
	return (args);
}

static char					**get_assignments(t_token *list, int length)
{
	int		i;
	int		size;
	char	**assignments;
	t_token	*copy;

	i = 0;
	size = 0;
	copy = list;
	while (copy && i < length && copy->type == TOKEN_ASSIGNMENT_WORD)
	{
		if (copy->type == TOKEN_ASSIGNMENT_WORD)
			size++;
		copy = copy->next;
		i++;
	}
	assignments = (char **)ft_memalloc(sizeof(char *) * (size + 1));
	i = 0;
	while (list && i < size)
	{
		if (list->type == TOKEN_ASSIGNMENT_WORD)
			assignments[i++] = ft_strdup(list->value);
		list = list->next;
	}
	assignments[size] = NULL;
	return (assignments);
}

t_bresult					*simple_command_build(const t_state *state,
											struct s_result *last_build)
{
	t_node				*node;
	t_token				*list;
	t_bresult			*result;
	struct s_command	*command;
	const int			size = last_build->consumed;

	result = ft_memalloc(sizeof(t_bresult));
	list = offset_list(state->list_offset, -size);
	command = (struct s_command *)ft_memalloc(sizeof(struct s_command));
	command->args = get_args(list, size);
	command->assignments = get_assignments(list, size);
	command->io_redirects = get_redirects(list, size);
	command->is_async = false;
	node = ast_new_node(command, NODE_COMMAND);
	result->root = node;
	result->request = state->rule;
	return (result);
}
