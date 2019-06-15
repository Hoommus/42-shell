/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/15 13:49:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/15 13:51:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

t_pipe_segment		*pipe_segment_new(t_command *command, t_context *context, bool is_subshell)
{
	t_pipe_segment	*segment;

	segment = ft_memalloc(sizeof(t_pipe_segment));
	segment->command = command;
	segment->context = context;
	segment->needs_wait = is_subshell; // Seems like redundant
	return (segment);
}

t_pipe_segment		*pipeline_add_segment(t_pipe_segment **head, t_pipe_segment *segment)
{
	t_pipe_segment	*list;

	if (!head)
		return (NULL);
	if (*head == NULL)
		*head = segment;
	else
	{
		list = *head;
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
	return (*head);
}

void				pipeline_destroy(t_pipe_segment **pipeline)
{
	t_pipe_segment	*list;
	t_pipe_segment	*next;

	if (!pipeline || !*pipeline)
		return ;
	list = *pipeline;
	while (list)
	{
		next = list->next;
		context_deep_free(&list->context);
		ft_memdel((void **)list);
		list = next;
	}
	*pipeline = NULL;
}

int					pipeline_execute(t_pipe_segment *pipeline)
{
	int		status;

	status = 1;
	while (pipeline)
	{
		status = pipeline_execute_segment(pipeline);
		pipeline = pipeline->next;
	}
	return (status);
}


int					pipeline_execute_segment(t_pipe_segment *segment)
{
	return (segment == NULL);
}

