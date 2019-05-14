/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_simple_command_rdrs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 14:51:17 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/06 17:05:05 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"
#include "shell_script_parser.h"

#define IS_HEREDOC(t) ((t) == TOKEN_DLESS || (t) == TOKEN_DLESSDASH || IS_HW(t))
#define IS_HW(t) ((t) == TOKEN_TRILESS)

bool						is_redirect(t_token *t)
{
	return (t && (t->type == TOKEN_IO_NUMBER
				|| t->type == TOKEN_DGREAT
				|| t->type == TOKEN_GREAT
				|| t->type == TOKEN_LESS
				|| t->type == TOKEN_DLESS
				|| t->type == TOKEN_TRILESS
				|| t->type == TOKEN_GREATAND
				|| t->type == TOKEN_LESSAND
				|| t->type == TOKEN_DLESSDASH
				|| t->type == TOKEN_LESSGREAT
				|| t->type == TOKEN_CLOBBER));
}

static void					construct_redirect(t_token *pivot,
												struct s_io_redirect *rdr)
{
	rdr->type = pivot->type;
	rdr->left.fd = IS_HEREDOC(pivot->type) || rdr->type == TOKEN_LESS ||
		rdr->type == TOKEN_LESSAND ? 0 : 1;
	rdr->right.fd = 0;
	if (pivot->prev && is_string_numeric(pivot->prev->value, 10))
		rdr->left.fd = ft_atoi(pivot->prev->value);
	if (pivot->next && is_string_numeric(pivot->next->value, 10) &&
		(pivot->type == TOKEN_GREATAND || pivot->type == TOKEN_LESSAND))
		rdr->right.fd = ft_atoi(pivot->next->value);
	else
		rdr->right.path = ft_strdup(pivot->next->value);
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
