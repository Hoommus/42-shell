/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_manipulations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 16:10:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:27:50 by vtarasiu         ###   ########.fr       */
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

	tmp = ft_memalloc(sizeof(struct s_fd_lst));
	if (label)
		tmp->label = ft_strdup(label);
	tmp->original = (short)original;
	tmp->current = (short)actual;
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
	ultimate = context->fd_list->next;
	if (penultimate->original == original)
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
