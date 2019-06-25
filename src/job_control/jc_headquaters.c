/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_headquaters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:49:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/20 15:04:18 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

const char				*g_state_names[] =
{
	"JOB_FG",
	"JOB_BG",
	"JOB_STOPPED",
	"JOB_SIGTTXX",
	"JOB_SIGTTOU",
	"JOB_EXITED",
	"JOB_PIPE",
	"JOB_TERMINATED"
};

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
	int			i;

	i = 1;
	list = jc_get()->active_jobs;
	if (list == NULL)
	{
		if (job->state == JOB_BG)
			ft_dprintf(2, "+[%d] %d\n", i, job->pgid);
		jc_get()->active_jobs = job;
		return ;
	}
	while (list->next)
	{
		list = list->next;
		i++;
	}
	if (job->state == JOB_BG)
		ft_dprintf(2, "+[%d] %d\n", i, job->pgid);
	list->next = job;
}

void						jc_unregister_job(pid_t id)
{
	t_job		*ultimate;
	t_job		*penultimate;

	penultimate = jc_get()->active_jobs;
	ultimate = penultimate ? jc_get()->active_jobs->next : NULL;
	if (penultimate && ultimate == NULL && penultimate->pgid == id)
	{
		jc_job_dealloc(&penultimate);
		jc_get()->active_jobs = NULL;
	}
	while (penultimate && ultimate)
	{
		if (ultimate->pgid == id)
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
