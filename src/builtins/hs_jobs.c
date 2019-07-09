/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 19:57:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 16:16:07 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"

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
	if (jc_is_subshell())
		return (1);
	if (args && args[0] && ft_strcmp(args[0], "killall") == 0)
	{
		hs_jobs_killall();
		return (0);
	}
	jc_check_n_notify(true);
	args = NULL;
	return (0);
}
