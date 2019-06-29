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

enum e_job_state	poll_pipeline(t_job *job, bool wnohang)
{
	t_proc			*procs;
	const int		wflags = WUNTRACED | (wnohang ? WNOHANG : 0);

	procs = job->procs;
	while (procs)
	{
		if (waitpid(procs->pid, &procs->status, wflags) == procs->pid)
		{
			job->notified = false;
			if (WIFEXITED(procs->status))
			{
				job->state = JOB_TERMINATED;
				procs->is_completed = true;
			}
			else if (WIFSTOPPED(procs->status))
			{
				job->state = WSTOPSIG(procs->status) > 0
							? WSTOPSIG(procs->status) + 30 : JOB_STOPPED;
				procs->is_stopped = true;
				if (!wnohang)
					break ;
			}
			else if (WIFSIGNALED(procs->status))
			{
				job->state = WTERMSIG(procs->status) + 30;
				procs->is_completed = true;
				if (!wnohang)
					break ;
			}
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

void				check_n_notify(bool notify_all)
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
		if (notify_all || (job->state != JOB_RUNNING && !job->notified++))
			jc_format_job(job);
		if (is_completed(job))
			jc_unregister_job(job->pgid);
		job = swap;
	}
}

int			hs_jobs(const char **args)
{
	check_n_notify(true);
	args = NULL;
	return (0);
}
