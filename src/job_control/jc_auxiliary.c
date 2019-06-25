/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_auxiliary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 17:12:08 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/22 17:13:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

void		jc_format_job(t_job *job)
{
	ft_dprintf(2, "[%d]    %d %-14s %s\n",
		job->id, job->pgid, jc_state_str(job->state), job->command);
}

char		*jc_state_str(enum e_job_state state)
{
	switch (state)
	{
		case JOB_EXITED:
		case JOB_TERMINATED:
			return ("terminated");
		case JOB_STOPPED:
			return ("stopped");
		case JOB_BG:
		case JOB_RUNNING:
			return ("running");
		case JOB_SIGTTXX:
		case JOB_SIGTTOU:
			return ("signalled");
		default:
			return ("unknown");
	}
}
