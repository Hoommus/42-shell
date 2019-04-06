/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_manipulations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 16:10:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/20 12:23:09 by vtarasiu         ###   ########.fr       */
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

void	context_remove_fd(t_context *context, const int fd)
{
	struct s_fd_lst	*swap;
	struct s_fd_lst	*last;

	last = context->fd_list;
	swap = context->fd_list->next;
	if (last->current == fd)
	{
		ft_memdel((void **)&(last));
		context->fd_list = swap;
		return ;
	}
	while (swap)
	{
		if (swap->current == fd)
		{
			last->next = swap->next;
			swap = last->next;
			ft_memdel((void **)&swap);
		}
		else
			swap = swap->next;
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