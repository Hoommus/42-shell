/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_switch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 12:09:35 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 18:05:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** If to_which is NULL, restores context from backup with pretty much the same
** algorithm
**
** Remember that dup2 does nothing, if arguments are equal, so there will not be
** any excessive duplications, if context was shallow duplicated
*/

void			context_switch(t_context *to_which)
{
	struct s_fd_lst		*list;
	int					flags;

	if (!to_which)
		to_which = g_term->context_backup;
	else
		g_term->context_backup = g_term->context_current;
	g_term->context_current = to_which;
	list = to_which->fd_list;
	while (list)
	{
		if (list->current == -1)
			close(list->original);
		else
		{
			dup2(list->current, list->original);
			flags = fcntl(list->current, F_GETFL);
			if (flags != -1)
				fcntl(list->current, F_SETFL, flags | O_CLOEXEC);
		}
		list = list->next;
	}
	TERM_APPLY_CONFIG(to_which->term_config);
}

static void		duplicate_environ(t_context *new_context,
	const t_context *context)
{
	t_var		var;
	u_int32_t	i;

	new_context->environ = environ_create_vector(context->environ->capacity);
	i = new_context->environ->size;
	new_context->environ->size = context->environ->size;
	while (i < new_context->environ->size)
	{
		ft_bzero(&var, sizeof(t_var));
		var.key = ft_strdup(((t_var *)context->environ->array + i)->key);
		var.value = ft_strdup(((t_var *)context->environ->array + i)->value);
		var.scope = ((t_var *)context->environ->array + i)->scope;
		var.hash = ((t_var *)context->environ->array + i)->hash;
		var.key_hash = ((t_var *)context->environ->array + i)->key_hash;
		((t_var *)new_context->environ->array)[i] = var;
		i++;
	}
}

struct s_fd_lst	*get_tmp_node(struct s_fd_lst *list, bool with_dup)
{
	struct s_fd_lst		*tmp;

	tmp = ft_memalloc(sizeof(struct s_fd_lst));
	if (list->label)
		tmp->label = ft_strdup("cloned");
	tmp->original = list->original;
	tmp->current = with_dup ? dup(list->current) : list->current;
	return (tmp);
}

static void		duplicate_fds(t_context *new, const t_context *context,
	bool with_dup)
{
	struct s_fd_lst		*list;
	struct s_fd_lst		*new_list;
	struct s_fd_lst		*tmp;

	new_list = NULL;
	list = context->fd_list;
	while (list)
	{
		if (list->current >= 0)
		{
			tmp = get_tmp_node(list, with_dup);
			if (!new->fd_list)
			{
				new->fd_list = tmp;
				new_list = tmp;
			}
			else
			{
				new_list->next = tmp;
				new_list = new_list->next;
			}
		}
		list = list->next;
	}
}

/*
** Creates a full copy of provided context
** If with_dup is set to 'true', duplicates all fds with corresponding syscall
** and duplicates environ.
** Otherwise, new context will share same file descriptors as "parent" context
** and the same pointer to environ (to modify shell environment without hassle).
*/

t_context		*context_duplicate(const t_context *context, int dup_what)
{
	t_context			*new;

	new = ft_memalloc(sizeof(t_context));
	new->term_config = ft_memalloc(sizeof(struct termios));
	if (context->term_config != NULL)
		ft_memcpy(new->term_config, context->term_config,
			sizeof(struct termios));
	if (dup_what & XDUP_ENV)
		duplicate_environ(new, context);
	else
		new->environ = environ_create_vector(4);
	if (dup_what & XDUP_FDS)
		duplicate_fds(new, context, dup_what);
	return (new);
}
