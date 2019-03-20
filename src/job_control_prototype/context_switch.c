/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_switch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 12:09:35 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/19 18:54:48 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

t_context		*context_init(void)
{
	t_context	*dummy;

	dummy = ft_memalloc(sizeof(t_context));
	dummy->environ = environ_create_vector(VAR_VECTOR_INITIAL_SIZE);
	return (dummy);
}

void			context_deep_free(t_context **context)
{
	t_var				*vars;
	struct s_fd_lst		*list;
	struct s_fd_lst		*swap;
	u_int32_t			i;

	vars = (*context)->environ->array;
	i = 0;
	while (i < (*context)->environ->size)
	{
		ft_memdel((void **)&(vars[i].key));
		ft_memdel((void **)&(vars[i].value));
		i++;
	}
	ft_memdel((void **)&((*context)->environ->array));
	ft_memdel((void **)&((*context)->environ));
	ft_memdel((void **)&((*context)->term_config));
	list = (*context)->fd_list;
	while (list)
	{
		swap = list->next;
		close(list->current);
		ft_memdel((void **)&(list->label));
		ft_memdel((void **)&list);
		list = swap;
	}
	ft_memdel((void **)context);
}

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

	if (!to_which)
		to_which = g_term->context_backup;
	else
		g_term->context_backup = g_term->context_current;
	g_term->context_current = to_which;
	list = to_which->fd_list;
	while (list)
	{
		dup2(list->current, list->original);
		list = list->next;
	}
	TERM_APPLY_CONFIG(to_which->term_config);
}

static void		duplicate_environ(t_context *new, const t_context *context)
{
	t_var		var;
	u_int32_t	i;

	new->environ = environ_create_vector(context->environ->capacity);
	new->environ->size = context->environ->size;
	i = 0;
	while (i < new->environ->size)
	{
		ft_bzero(&var, sizeof(t_var));
		var.key = ft_strdup(((t_var *)context->environ->array + i)->key);
		var.value = ft_strdup(((t_var *)context->environ->array + i)->value);
		var.scope = ((t_var *)context->environ->array + i)->scope;
		var.hash = ((t_var *)context->environ->array + i)->hash;
		var.key_hash = ((t_var *)context->environ->array + i)->key_hash;
		((t_var *)new->environ->array)[i] = var;
		i++;
	}
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
		tmp = ft_memalloc(sizeof(struct s_fd_lst));
		if (list->label)
			tmp->label = ft_strdup(list->label);
		tmp->original = list->original;
		tmp->current = with_dup ? (short)dup(list->current) : list->current;
		if (!new->fd_list && !new_list)
		{
			new->fd_list = tmp;
			new_list = tmp;
		}
		else
		{
			new_list->next = tmp;
			new_list = new_list->next;
		}
		list = list->next;
	}
}

/*
** Creates a full copy of provided context
** If with_dup is set to 'true', duplicates all fds with corresponding syscall.
** Otherwise, new context will share same file descriptors as "parent" context
*/

t_context		*context_duplicate(const t_context *context, bool with_dup)
{
	t_context			*new;

	new = ft_memalloc(sizeof(t_context));
	new->term_config = ft_memalloc(sizeof(struct termios));
	if (context->term_config != NULL)
		ft_memcpy(new->term_config, context->term_config, sizeof(struct termios));
	duplicate_environ(new, context);
	duplicate_fds(new, context, with_dup);
	return (new);
}
