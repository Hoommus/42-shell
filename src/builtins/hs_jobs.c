/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 19:57:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/27 18:55:01 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"

int					alterate_proc(t_job *job, t_proc *proc)
{
	job->notified = false;
	if (WIFEXITED(proc->status))
	{
		job->state = JOB_TERMINATED;
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
	return (0);
}

enum e_job_state	poll_pipeline(t_job *job, bool wnohang)
{
	t_proc			*procs;
	const int		wflags = WUNTRACED | (wnohang ? WNOHANG : 0);

	procs = job->procs;
	while (procs)
	{
		if (waitpid(procs->pid, &procs->status, wflags) == procs->pid)
		{
			if (alterate_proc(job, procs) >= 2)
				break ;
		}
		procs = procs->next;
	}
	return (job->state);
}

bool				is_completed(t_job *job)
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
	return (is_completed);
}

void				jc_check_n_notify(bool notify_all)
{
	t_job				*job;
	t_job				*swap;
	enum e_job_state	state;

	if (g_term->fallback_input_state == STATE_NON_INTERACTIVE)
		return ;
	job = jc_get()->active_jobs;
	while (job)
	{
		swap = job->next;
		state = poll_pipeline(job, true);
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

void				hs_jobs_killall(void)
{
	t_job	*jobs;
	t_proc	*procs;

	jobs = jc_get()->active_jobs;
	while (jobs)
	{
		procs = jobs->procs;
		while (procs)
		{
			kill(procs->pid, SIGKILL);
			procs = procs->next;
		}
		jobs = jobs->next;
	}
}

int					hs_jobs(const char **args)
{
	if (args && args[0] && ft_strcmp(args[0], "killall") == 0)
	{
		hs_jobs_killall();
	}
	jc_check_n_notify(true);
	args = NULL;
	return (0);
}
