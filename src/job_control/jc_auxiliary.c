/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_auxiliary.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/22 17:12:08 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/27 18:43:47 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

void		jc_format_job(const t_job *job)
{
	char	option;

	if (job->next == NULL)
		option = '+';
	else if (job->next && job->next->next == NULL)
		option = '-';
	else
		option = ' ';
	ft_dprintf(2, "[%d]  %c %d %-14s %s\n",
		job->id, option, job->pgid, jc_state_str(job->state), job->command);
}

char		*jc_state_str(enum e_job_state state)
{
	extern char		*g_sigs[];

	if (state == JOB_EXITED || state == JOB_TERMINATED)
		return ("terminated");
	else if (state == JOB_STOPPED)
		return ("stopped");
	else if (state == JOB_RUNNING || state == JOB_BG)
		return ("running");
	else if (state > 31)
		return (g_sigs[state - 31]);
	else if (state == JOB_LAUNCHED)
		return ("launched");
	else if (state == JOB_CONTINUED)
		return ("continued");
	else
		return ("unknown");
}

void					close_redundant_fds(t_context *context)
{
	struct s_fd_lst		*list;

	if (context == NULL)
		return ;
	list = context->fd_list;
	while (list)
	{
		if (ft_strcmp(list->label, "rdr_duped_1337"))
		{
			if (list->current > 2)
				close(list->current);
		}
		list = list->next;
	}
}
