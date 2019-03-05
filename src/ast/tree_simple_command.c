/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_simple_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 15:57:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/04 18:48:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

static bool					is_redirect(t_token *t)
{
	return (t && (t->type == TOKEN_IO_NUMBER
			|| t->type == TOKEN_DGREAT
			|| t->type == TOKEN_GREAT
			|| t->type == TOKEN_DLESS
			|| t->type == TOKEN_LESS
			|| t->type == TOKEN_GREATAND
			|| t->type == TOKEN_LESSAND
			|| t->type == TOKEN_DLESSDASH
			|| t->type == TOKEN_CLOBBER));
}

static char					**get_args(t_token *list, int length)
{
	char	**args;
	int		i;
	int		size;
	t_token	*copy;

	i = -1;
	size = 0;
	copy = list;
	while (copy && ++i < length)
	{
		if (copy->type == TOKEN_WORD
			&& !(is_redirect(copy->next) || is_redirect(copy->prev)))
			size++;
		copy = copy->next;
	}
	args = ft_memalloc(sizeof(char *) * (size + 1));
	i = -1;
	copy = list;
	while (copy && ++i < length)
	{
		if (copy->type == TOKEN_WORD
			&& !(is_redirect(copy->next) || is_redirect(copy->prev)))
			args[i] = ft_strdup(copy->value);
		copy = copy->next;
	}
	return (args);
}

static void					construct_redirect(t_token *pivot,
	struct s_io_redirect *rdr)
{
	bool	is_left;

	rdr->type = pivot->type;
	is_left = rdr->type == TOKEN_DLESS || rdr->type == TOKEN_LESSAND
			|| rdr->type == TOKEN_LESS || rdr->type == TOKEN_DLESSDASH;
	rdr->append = rdr->type == TOKEN_DGREAT;
	rdr->what.fd = 1;
	rdr->where.fd = 0;
	if (is_left)
	{
		rdr->where.fd = ft_atoi(pivot->prev->value);
		rdr->what.string = ft_strdup(pivot->next->value);
	}
	else
	{
		rdr->what.fd = (ft_atoi(pivot->prev->value) == 0 ? 1
											: ft_atoi(pivot->prev->value));
		if (pivot->next->type == TOKEN_WORD)
			rdr->where.string = ft_strdup(pivot->next->value);
	}
}

static struct s_io_redirect	**get_redirects(t_token *list, int length)
{
	struct s_io_redirect	**array;
	t_token					*copy;
	int						i;
	int						size;

	i = 0;
	size = 0;
	copy = list;
	while (copy && i < length)
	{
		if (is_redirect(copy))
			size++;
		copy = copy->next;
		i++;
	}
	array = ft_memalloc(sizeof(struct s_io_redirect *) * (size + 1));
	copy = list;
	i = 0;
	while (copy && i < size && length--)
	{
		if (is_redirect(copy))
			construct_redirect(copy, array[i++] =
				ft_memalloc(sizeof(struct s_io_redirect)));
		copy = copy->next;
	}
	return (array);
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
	copy = list;
	i = 0;
	while (copy && i < size)
	{
		if (copy->type == TOKEN_ASSIGNMENT_WORD)
			assignments[i++] = ft_strdup(copy->value);
		copy = copy->next;
	}
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
	// Seems like a dirty hack or simply not working solution
	// TODO: consider removing
	command->is_async = offset_list(list, size - 1) == NULL ? false :
						offset_list(list, size - 1)->type == TOKEN_AMPERSAND;
	node = ast_new_node(command, NODE_COMMAND);
	result->root = node;
	result->request = state->rule;
	return (result);
}


