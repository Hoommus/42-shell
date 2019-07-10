/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_job_launcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 04:03:21 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 14:43:23 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "shell_job_control.h"
#include "shell_builtins.h"

int				jc_wait(t_job *job)
{
	t_proc		*procs;
	int			status;

	poll_pipeline(job, 0);
	procs = job->procs;
	while (procs->next)
		procs = procs->next;
	alterate_proc(job, procs);
	status = procs->status;
	if (job->state == JOB_SIGTSTP || job->state == JOB_SIGSTOP ||
		job->state == JOB_SIGTTIN || job->state == JOB_SIGTTOU)
		return (-1024);
	return (status);
}

int				jc_resolve_status(t_job *job)
{
	t_proc		*proc;

	proc = job->procs;
	while (proc->next)
		proc = proc->next;
	if (proc->pid != 0 && WIFSIGNALED(proc->status))
	{
		job->state = WTERMSIG(proc->status) + 30;
		ft_dprintf(2, "%s %s\n", jc_state_str(job->state), job->command);
		return (WTERMSIG(proc->status));
	}
	else if (proc->pid != 0 && WIFSTOPPED(proc->status))
	{
		job->state = WSTOPSIG(proc->status) > 0
					? WSTOPSIG(proc->status) + 30 : JOB_STOPPED;
		return (WSTOPSIG(proc->status));
	}
	else if (proc->pid != 0 && WIFEXITED(proc->status))
	{
		job->state = JOB_TERMINATED;
		return (WEXITSTATUS(proc->status));
	}
	else if (proc->pid == 0)
		return (proc->status);
	return (-2048);
}

int				jc_launch(t_job *job, bool is_async)
{
	int				status;

	if (job->procs && job->procs->command)
		job->command = ft_strarr_join(" ", job->procs->command->args);
	status = execute_segments(job, is_async);
	if (!is_async)
	{
		tcsetpgrp(0, g_term->shell_pgid);
		status = jc_resolve_status(job);
		status = status == -2048 ? 127 : status;
	}
	tcsetattr(0, TCSADRAIN, g_term->shell_term);
	if (!jc_is_subshell() && (is_async || status == -1024))
	{
		poll_pipeline(job, WNOHANG | WCONTINUED);
		jc_register_job(job);
	}
	else if (job->state == JOB_SIGTSTP || job->state == JOB_SIGSTOP ||
			job->state == JOB_SIGTTIN || job->state == JOB_SIGTTOU)
		jc_register_job(job);
	else
		jc_job_dealloc(&(job));
	return (status);
}
