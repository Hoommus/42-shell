/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_simple_command_rdrs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 14:51:17 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 15:03:03 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

bool						is_redirect(t_token *t)
{
	return (t && (t->type == TOKEN_IO_NUMBER
				|| t->type == TOKEN_DGREAT
				|| t->type == TOKEN_GREAT
				|| t->type == TOKEN_DLESS
				|| t->type == TOKEN_LESS
				|| t->type == TOKEN_GREATAND
				|| t->type == TOKEN_LESSAND
				|| t->type == TOKEN_DLESSDASH
				|| t->type == TOKEN_LESSGREAT
				|| t->type == TOKEN_CLOBBER));
}

static void					construct_redirect(t_token *pivot,
												struct s_io_redirect *rdr)
{
	bool	is_left;

	rdr->type = pivot->type;
	is_left = rdr->type == TOKEN_DLESS || rdr->type == TOKEN_LESSAND
			|| rdr->type == TOKEN_LESS || rdr->type == TOKEN_DLESSDASH;
	rdr->what.fd = 1;
	rdr->where.fd = 0;
	if (is_left)
	{
		if (is_string_numeric(pivot->prev->value, 10))
			rdr->where.fd = ft_atoi(pivot->prev->value);
		rdr->what.path = ft_strdup(pivot->next->value);
	}
	else
	{
		if (is_string_numeric(pivot->prev->value, 10))
			rdr->what.fd = ft_atoi(pivot->prev->value);
		if (pivot->next && pivot->next->type == TOKEN_WORD)
			rdr->where.path = ft_strdup(pivot->next->value);
	}
}

struct s_io_redirect		*get_redirects(t_token *list, int length)
{
	struct s_io_redirect	*array;
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
	array = ft_memalloc(sizeof(struct s_io_redirect) * (size + 1));
	array[size].type = TOKEN_NOT_APPLICABLE;
	i = 0;
	while (list && i < size && length--)
	{
		if (is_redirect(list))
			construct_redirect(list, array + i++);
		list = list->next;
	}
	return (array);
}
