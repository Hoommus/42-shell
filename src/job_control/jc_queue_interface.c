/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_interface.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 15:39:07 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/04 12:57:51 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

void						jc_enqueue_job(t_job *job)
{
	t_job		*list;

	list = jc_get()->job_queue;
	if (list == NULL)
	{
		jc_get()->job_queue = job;
		jc_get()->queue_size++;
		return ;
	}
	while (list->next)
		list = list->next;
	list->next = job;
	job->prev = list;
	job->next = NULL;
	jc_get()->queue_size++;
}

/*
** Function searches for a job using either pid or job->pid value, so no need
** for supplying both.
*/

t_job						*jc_dequeue_job(pid_t pid, t_job *job)
{
	t_job		*list;

	if (pid <= 0 && job == NULL)
		return (NULL);
	list = jc_get()->job_queue;
	while (list)
	{
		if (list->pid == pid || (job && job->pid == list->pid))
		{
			if (list->prev)
				list->prev->next = list->next;
			else
				jc_get()->jobs = list->next;
			return (list);
		}
	}
	return (NULL);
}

/*
** That wait() call will break when implementing correct Job Control
*/

void						jc_destroy_queue(void)
{
	t_job		*next;
	t_job		*list;
	int			status;

	list = jc_get()->job_queue;
	while (list)
	{
		wait(&status);
		next = list->next;
		context_deep_free(&(list->context));
		ft_memdel((void **)&list);
		list = next;
	}
	jc_get()->queue_size = 0;
	jc_get()->job_queue = NULL;
}
