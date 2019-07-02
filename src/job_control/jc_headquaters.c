/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_headquaters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:49:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/27 20:03:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

static struct s_job_control	g_jc;

void						jc_init(t_context *context)
{
	ft_bzero(&g_jc, sizeof(struct s_job_control));
	g_jc.shell_pid = getpid();
	g_jc.shell_context = context;
}

struct s_job_control		*jc_get(void)
{
	return (&g_jc);
}

void						jc_register_job(t_job *job)
{
	t_job	*list;
	int		i;

	i = 1;
	list = jc_get()->active_jobs;
	job->id = i;
	if (list == NULL)
		jc_get()->active_jobs = job;
	else
	{
		while (list->next)
		{
			list = list->next;
			i++;
		}
		job->id = i + 1;
		list->next = job;
	}
	jc_format_job(job);
}

void						jc_unregister_job(pid_t pgid)
{
	t_job		*ultimate;
	t_job		*penultimate;

	penultimate = jc_get()->active_jobs;
	ultimate = penultimate ? jc_get()->active_jobs->next : NULL;
	if (penultimate && ultimate == NULL && penultimate->pgid == pgid)
	{
		jc_job_dealloc(&penultimate);
		jc_get()->active_jobs = NULL;
	}
	while (penultimate && ultimate)
	{
		if (ultimate->pgid == pgid)
		{
			if (ultimate == jc_get()->active_jobs)
				jc_get()->active_jobs = ultimate->next;
			else
				penultimate->next = ultimate->next;
			jc_job_dealloc(&ultimate);
			return ;
		}
		penultimate = penultimate->next;
		ultimate = ultimate->next;
	}
}
