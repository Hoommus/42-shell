/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_headquaters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:49:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/19 17:50:45 by vtarasiu         ###   ########.fr       */
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

t_job						*jc_create_job(const struct s_command *cmd,
	t_context *context, enum e_job_state job_class)
{
	t_job		*job;

	job = ft_memalloc(sizeof(t_job));
	job->context = context;
	job->state = job_class;
	job->cmd = cmd;
	return (job);
}

void						jc_register_job(t_job *job)
{
	t_job	*list;
	int		i;

	i = 1;
	list = jc_get()->jobs;
	if (list == NULL)
	{
		if (job->state == JOB_BG)
			ft_dprintf(2, "[%d] %d\n", i, job->pid);
		jc_get()->jobs = job;
		return ;
	}
	while (list->next)
	{
		list = list->next;
		i++;
	}
	if (job->state == JOB_BG)
		ft_dprintf(2, "[%d] %d\n", i, job->pid);
	list->next = job;
	job->prev = list;
}

void						jc_unregister_job(pid_t id)
{
	t_job		*list;

	list = jc_get()->jobs;
	while (list)
	{
		if (list->pid == id)
		{
			if (list->prev)
				list->prev->next = list->next;
			else
				jc_get()->jobs = list->next;
			context_deep_free(&(list->context));
			ft_memdel((void **)&list);
			return ;
		}
		list = list->next;
	}
}
