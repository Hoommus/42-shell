/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_manipulations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 16:10:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 18:11:53 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void	context_add_fd(t_context *context,
						const int original,
						const int actual,
						const char *label)
{
	struct s_fd_lst	*swap;
	struct s_fd_lst	*tmp;
	int				flags;

	tmp = ft_memalloc(sizeof(struct s_fd_lst));
	if (label)
		tmp->label = ft_strdup(label);
	tmp->original = (short)original;
	tmp->current = (short)actual;
	flags = fcntl(actual, F_GETFL);
	if (flags != -1)
		fcntl(actual, F_SETFL, flags | O_CLOEXEC);
	swap = context->fd_list;
	if (!swap)
		context->fd_list = tmp;
	else
	{
		while (swap->next)
			swap = swap->next;
		swap->next = tmp;
	}
}

void	context_remove_ofd(t_context *context, const int original)
{
	struct s_fd_lst	*ultimate;
	struct s_fd_lst	*penultimate;

	penultimate = context->fd_list;
	ultimate = context->fd_list ? context->fd_list->next : NULL;
	if (penultimate && penultimate->original == original)
	{
		close(penultimate->current);
		ft_memdel((void **)&(penultimate->label));
		ft_memdel((void **)&(penultimate));
		context->fd_list = ultimate;
		return ;
	}
	while (ultimate)
	{
		if (ultimate->original == original)
		{
			close(ultimate->current);
			penultimate->next = ultimate->next;
			ft_memdel((void **)&(ultimate->label));
			return (ft_memdel((void **)&ultimate));
		}
		penultimate = ultimate;
		ultimate = ultimate->next;
	}
}

void	context_remove_fd(t_context *context, const int fd)
{
	struct s_fd_lst	*ultimate;
	struct s_fd_lst	*penultimate;

	penultimate = context->fd_list;
	ultimate = context->fd_list->next;
	if (penultimate->current == fd)
	{
		ft_memdel((void **)&(penultimate->label));
		ft_memdel((void **)&(penultimate));
		context->fd_list = ultimate;
		return ;
	}
	while (ultimate)
	{
		if (ultimate->current == fd)
		{
			penultimate->next = ultimate->next;
			ft_memdel((void **)&(ultimate->label));
			ft_memdel((void **)&ultimate);
			return ;
		}
		penultimate = ultimate;
		ultimate = ultimate->next;
	}
}

bool	context_is_fd_present(const t_context *context, const int original)
{
	struct s_fd_lst		*list;

	list = context->fd_list;
	while (list)
	{
		if (list->original == original)
			return (true);
		list = list->next;
	}
	return (false);
}

void	context_deep_free(t_context **context)
{
	struct s_fd_lst		*list;
	struct s_fd_lst		*swap;

	if (context == NULL || *context == NULL)
		return ;
	environ_deallocate_vector((*context)->environ);
	list = (*context)->fd_list;
	while (list)
	{
		swap = list->next;
		if (list->current > 2)
			close(list->current);
		free(list->label);
		free(list);
		list = swap;
	}
	ft_memdel((void **)&((*context)->term_config));
	ft_memdel((void **)context);
}
