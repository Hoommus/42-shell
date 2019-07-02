/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/15 13:49:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/18 15:09:52 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

t_proc		*process_create(t_command *command, t_context *context)
{
	t_proc	*segment;

	segment = ft_memalloc(sizeof(t_proc));
	segment->command = command;
	segment->context = context;
	return (segment);
}

t_proc		*process_list_add(t_proc **pipeline, t_proc *segment)
{
	t_proc	*list;

	if (!pipeline)
		return (NULL);
	if (*pipeline == NULL)
		*pipeline = segment;
	else
	{
		list = *pipeline;
		while (list)
		{
			if (list->next == NULL)
			{
				list->next = segment;
				break;
			}
			list = list->next;
		}
	}
	return (*pipeline);
}

void				process_list_destroy(t_proc **pipeline)
{
	t_proc	*list;
	t_proc	*next;

	if (!pipeline || !*pipeline)
		return ;
	list = *pipeline;
	while (list)
	{
		next = list->next;
		context_deep_free(&(list->context));
		ft_memdel((void **)&list);
		list = next;
	}
	*pipeline = NULL;
}
