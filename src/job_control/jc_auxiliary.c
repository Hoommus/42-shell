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

static const char	*g_sigs[] =
{
	"hangup",
	"interrupted",
	"quit",
	"illegal instruction",
	"trace trap",
	"abort",
	"EMT instruction",
	"floating point exception",
	"killed",
	"bus error",
	"segmentation fault",
	"invalid system call",
	"broken pipe",
	"suspended (tty input)",
	"terminated",
	"suspended (tty input)",
	"suspended (signal)",
	"suspended",
	"suspended (tty input)",
	"suspended (tty input)",
	"suspended (tty input)",
	"suspended (tty output)",
	"suspended (tty input)",
	"cpu limit exceeded",
	"file size limit exceeded",
	"virtual time alarm",
	"profile signal",
	"suspended (tty input)",
	"suspended (tty input)",
	"user-defined signal 1",
	"user-defined signal 2"
};

void				jc_format_job(const t_job *job)
{
	char	option;

	if (job->next == NULL)
		option = '+';
	else if (job->next && job->next->next == NULL)
		option = '-';
	else
		option = ' ';
	ft_dprintf(2, "[%d%-*c %c %d %-14s %s\n",
		job->id, job->id > 10 ? 3 : 2, ']', option, job->pgid,
		jc_state_str(job->state), job->command);
}

const char			*jc_state_str(enum e_job_state state)
{
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

void				close_redundant_fds(t_context *context)
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
