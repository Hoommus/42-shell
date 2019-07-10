/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_bg.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 03:42:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 12:28:02 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

void		jc_to_bg(t_job *job)
{
	t_proc				*segments;

	tcsetpgrp(0, g_term->shell_pgid);
	segments = job->procs;
	kill(-job->pgid, SIGCONT);
	while (segments)
	{
		if (segments->is_stopped && !segments->is_completed)
		{
			segments->is_stopped = false;
			segments->is_completed = false;
			job->notified = false;
		}
		segments = segments->next;
	}
	poll_pipeline(job, WNOHANG | WCONTINUED);
}

int			hs_bg(const char **args)
{
	t_job		*job;
	const char	*arg;

	if (jc_is_subshell())
		return (1);
	if (!args || !args[0] || args[0][0] != '%' || (args[0] && args[1]))
	{
		ft_dprintf(2, "bg: invalid argument\n"
			"usage:\n    bg [job_id]\n");
		return (1);
	}
	if (jc_get()->active_jobs == NULL)
		return ((ft_dprintf(2, "bg: no active jobs\n") & 0) | 1);
	arg = args[0] + 1;
	job = choose_job(arg);
	if (job == NULL)
	{
		return ((ft_dprintf(2, "bg: no jobs matching criteria: "
			"`%%%s'\n", arg) & 0) | 1);
	}
	jc_to_bg(job);
	return (0);
}
