/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_jobs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 19:57:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/21 19:17:15 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"

int			hs_jobs(const char **args)
{
	t_job	*jobs = jc_get()->active_jobs;

	while (jobs)
	{
		ft_printf("[%d]  + %d %s %s\n", jobs->id, jobs->pgid, "stopped", jobs->command);
		jobs = jobs->next;
	}
	args = NULL;
	return (0);
}
