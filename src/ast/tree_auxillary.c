/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_auxillary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:57:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/15 15:57:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"
#include "shell_script_builders.h"

static char			**get_args_command(t_token *list, size_t length)
{
	char	**args;
	int		i;
	int		size;
	t_token	*copy;

	size = 0;
	while (list && list->type == TOKEN_ASSIGNMENT_WORD)
		list = list->next;
	while (list && size < length && list->type != TOKEN_IO_NUMBER)
	{
		list = list->next;
		size++;
	}
}

static t_token		*get_assignments(t_token **list, size_t length)
{
	int		i;
	int		size;
	t_token	*assignments;
	t_token	*copy;

	i = 0;
	size = 0;
	copy = *list;
	while (copy && size < length && copy->type == TOKEN_ASSIGNMENT_WORD)
	{
		copy = copy->next;
		size++;
	}
	assignments = (t_token *)ft_memalloc(sizeof(t_token) * (size + 1));
	copy = *list;
	while (i < size)
	{
		assignments[i++] = *copy;
		copy = copy->next;
	}
	*list = offset_list(*list, size);
	return (assignments);
}

t_bresult			simple_command_build(t_token *list, size_t length)
{
	t_node				*node;
	int					i;
	t_token				copy;
	t_bresult			result;
	struct s_command	*command;

	command = (struct s_command *)ft_memalloc(sizeof(struct s_command));
	command->args = (char **)ft_memalloc(sizeof(char *) * (length + 1));
	command->assignments = get_assignments(list, length);
	i = 0;
	while (i < length)
	{

		command->args[i++] = list->value;
	}
	node = ast_new_node(command, TOKEN_NOT_APPLICABLE, NODE_COMMAND);
	result.ast_root = node;
	result.error = NULL;
	return (result);
}
