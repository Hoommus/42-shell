/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_forknrun.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 18:31:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/03 15:47:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_builtins.h"

static char				*g_sigs[31] =
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

void					close_redundant_fds(t_context *context)
{
	struct s_fd_lst		*list;

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


static void				close_foreign_fds(t_job *jobs, t_job *current)
{
	struct s_fd_lst	*list;

	while (jobs)
	{
		if (jobs != current)
		{
			list = jobs->context->fd_list;
			while (list)
			{
				if (list->current > 2)
					close(list->current);
				list = list->next;
			}
		}
		jobs = jobs->next;
	}
}

static void				handle_signaled(t_job *job, int status)
{
	char		*swap;
	t_job		*list;
	int			i;

	i = 1;
	list = jc_get()->jobs;
	while (list && ++i)
		list = list->next;
	swap = ft_strarr_join(" ", job->cmd->args);
	if (ft_strlen(swap) < 45)
		ft_printf("\n[%d]    %d %s   %s\n", i, job->pid,
				g_sigs[WTERMSIG(status) - 1], swap);
	else
		ft_printf("\n[%d]    %d %s   %.*s...\n", i, job->pid,
				g_sigs[WTERMSIG(status) - 1], 45, swap);
	ft_memdel((void **)&swap);
}

static void				write_heredocs(t_job *job)
{
	const t_io_rdr		*io_rdrs = job->cmd->io_redirects;

	while (io_rdrs->type != TOKEN_NOT_APPLICABLE)
	{
		if ((io_rdrs->type == TOKEN_DLESS || io_rdrs->type == TOKEN_DLESSDASH))
		{
			if (g_term->fallback_input_state != STATE_NON_INTERACTIVE)
				write(io_rdrs->what.fd, io_rdrs->what.path,
					ft_strlen(io_rdrs->what.path));
			write(io_rdrs->what.fd, "\n", 1);
			close(io_rdrs->what.fd);
		}
		else if (io_rdrs->type == TOKEN_TRILESS)
		{
			write(io_rdrs->what.fd, io_rdrs->what.path,
					ft_strlen(io_rdrs->what.path));
			write(io_rdrs->what.fd, "\n", 1);
			close(io_rdrs->what.fd);
		}
		io_rdrs++;
	}
}

static int				waitnclaim(t_job *last)
{
	int			status;

	waitpid(last->pid, &status, 0);
	last->status = status;
	last->wexitstatus = WEXITSTATUS(last->status);
	if (WIFSIGNALED(status) && !WIFEXITED(status)
		&& WTERMSIG(status) != 1 && WTERMSIG(status) <= 31)
		handle_signaled(last, status);
	while (waitpid(-1, &status, WNOHANG) != -1)
		;
	return (last->wexitstatus);
}

int						forknrun(t_job *job, char *path)
{
	job->pid = fork();
	if (job->pid == 0)
	{
		context_switch(job->context);
		close_redundant_fds(job->context);
		close_foreign_fds(jc_get()->job_queue, job);
		execve(path, job->cmd->args, environ_to_array(job->context->environ,
			SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL));
		context_switch(jc_get()->shell_context);
		ft_dprintf(2, "21sh: execve error: %s\n", job->cmd->args[0]);
	}
	else if (job->pid == -1)
		ft_dprintf(2, "21sh: fork error: %s\n", job->cmd->args[0]);
	else
	{
		write_heredocs(job);
		close_redundant_fds(job->context);
		ft_memdel((void **)&path);
		if (job->next == NULL)
			return (waitnclaim(job));
	}
	return (-256);
}
