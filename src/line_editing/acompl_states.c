/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_states.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 21:02:58 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 14:45:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <shell_script.h>
#include "line_editing.h"
#include "twenty_one_sh.h"

static inline void		free_tokens(t_token *list)
{
	t_token		*next;

	while (list)
	{
		next = list->next;
		free_token(list);
		list = next;
	}
}

static inline t_token	*trim(t_token *list)
{
	t_token		*copy;
	t_token		*prev;

	copy = list;
	while (copy)
	{
		if (!copy->next && copy->type == TOKEN_NEWLINE)
		{
			prev = copy->prev;
			free_token(copy);
			prev->next = NULL;
			break ;
		}
		copy = copy->next;
	}
	while (list && list->next)
		list = list->next;
	return (list);
}

char					*acompl_update_state(u_int64_t from_index, char *buf,
	size_t bufsize)
{
	register t_token	*list;
	register t_token	*backup;
	char				*tmp;

	if (recheck_state(from_index) != STATE_NORMAL ||
		buff_char_at_equals_any(g_term->buffer->iterator, " "))
		return ((char *)(g_term->acompl_state = AC_NONE));
	tmp = buff_get_part(0, UINT64_MAX);
	backup = tokenize(tmp, TOKEN_DELIMITERS);
	list = trim(backup);
	if (list && list->type == TOKEN_WORD &&
		!ft_strchr_any(list->value, "'\"`${("))
	{
		if (list->prev == NULL)
			g_term->acompl_state = AC_COMMAND;
		else if (list->prev->type == TOKEN_WORD)
			g_term->acompl_state = AC_FILE;
		else if (list->prev->type >= TOKEN_TRILESS &&
			list->prev->type <= TOKEN_GREAT && list->prev->type != TOKEN_PIPE)
			g_term->acompl_state = AC_COMMAND;
		ft_strncpy(buf, list->value, bufsize);
	}
	free(tmp);
	free_tokens(backup);
	return (buf);
}
