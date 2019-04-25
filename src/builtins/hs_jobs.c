/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 19:57:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/19 17:50:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"

int			hs_jobs(const char **args)
{
	t_job				*jobs;
	int					i;

	i = 1;
	jobs = jc_get()->jobs;
	while (jobs)
	{
		if (flag_short_present(args, 'l'))
			ft_printf("[%.2d]  + %d %s %s\n", i++, jobs->pid,
				g_state_names[jobs->state], jobs->cmd->args[0]);
		else
			ft_printf("[%.2d]  + %d %s %s\n", i++, jobs->pid,
				g_state_names[jobs->state], jobs->cmd->args[0]);
		jobs = jobs->next;
	}
	return (0);
}
