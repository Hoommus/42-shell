/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_children_cleanup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:41:56 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 15:48:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

void		jc_remove_zombies(void)
{
	t_job		*list;

	list = jc_get()->jobs;
	while (list)
	{
		if (list->state == JOB_EXITED || list->state == JOB_TERMINATED)
		{
			if (list->prev)
				list->prev->next = list->next;
			else
				list = list->next;
		}
		else
			list = list->next;
	}
}

/*
** Function claims exit statuses and marks jobs for removal from job table.
** Should be called before prompt output
*/
void		jc_poll_jobs(void)
{
	t_job		*list;
	int			status;

	list = jc_get()->jobs;
	while (list)
	{
		waitpid(list->pid, &status, WNOHANG | WUNTRACED);
		if (WIFEXITED(status))
		{
			list->state = JOB_EXITED;
			list->exit_status = WEXITSTATUS(status);
		}
		if (WIFSIGNALED(status))
			list->state = JOB_TERMINATED;
		if (WIFSTOPPED(status))
			list->state = JOB_STOPPED;
		list = list->next;
	}
}