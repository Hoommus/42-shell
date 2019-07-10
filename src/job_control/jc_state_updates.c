/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_state_updates.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 04:09:15 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 19:10:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "shell_job_control.h"

static bool			is_completed(t_job *job)
{
	t_proc	*list = job->procs;
	bool	is_completed;

	is_completed = true;
	while (list)
	{
		if (!list->is_completed)
		{
			is_completed = false;
			break ;
		}
		list = list->next;
	}
	job->state = is_completed ? JOB_TERMINATED : job->state;
	return (job->state == JOB_TERMINATED || job->state == JOB_SIGKILL);
}

int					alterate_proc(t_job *job, t_proc *proc)
{
	job->notified = false;
	if (WIFEXITED(proc->status))
	{
		job->state = WTERMSIG(proc->status) > 0
					? WTERMSIG(proc->status) + 30 : JOB_TERMINATED;
		proc->is_completed = true;
		return (1);
	}
	else if (WIFSTOPPED(proc->status))
	{
		job->state = WSTOPSIG(proc->status) > 0
					? WSTOPSIG(proc->status) + 30 : JOB_STOPPED;
		proc->is_stopped = true;
		return (2);
	}
	else if (WIFSIGNALED(proc->status))
	{
		job->state = WTERMSIG(proc->status) + 30;
		proc->is_completed = true;
		return (3);
	}
	else if (WIFCONTINUED(proc->status))
	{
		job->state = JOB_CONTINUED;
		proc->is_completed = false;
		proc->is_stopped = false;
		return (1);
	}
	return (0);
}

enum e_job_state	poll_pipeline(t_job *job, int wait_flags)
{
	t_proc			*procs;
	const int		wflags = WUNTRACED | wait_flags;
	int				w;

	procs = job->procs;
	while (procs)
	{
		if (procs->pid != 0 && !procs->is_completed &&
			(w = waitpid(procs->pid, &procs->status, wflags)) == procs->pid)
		{
			if (alterate_proc(job, procs) >= 2 && (wflags & WNOHANG))
				break ;
		}
		procs = procs->next;
	}
	return (job->state);
}

void				jc_check_n_notify(bool notify_all)
{
	t_job				*job;
	t_job				*swap;

	if (g_term->fallback_input_state == STATE_NON_INTERACTIVE)
		return ;
	job = jc_get()->active_jobs;
	while (job)
	{
		swap = job->next;
		poll_pipeline(job, WNOHANG | WCONTINUED);
		if (notify_all || (job->state != JOB_RUNNING && !job->notified))
		{
			jc_format_job(job);
			job->notified = true;
		}
		if (is_completed(job))
			jc_unregister_job(job->pgid);
		job = swap;
	}
}
