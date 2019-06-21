/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_forknrun.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 18:31:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/20 18:05:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_builtins.h"

char					*g_sigs[31] =
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

void					handle_signaled(t_pipe_segment *process, int status)
{
	char			*swap;
	t_job_alt		*list;
	int				i;

	i = 1;
	list = jc_get()->active_jobs;
	while (list && ++i)
		list = list->next;
	swap = ft_strarr_join(" ", process->command->args);
	if (ft_strlen(swap) < 45)
		ft_printf("\n[%d]    %d %s   %s\n", i, process->pid,
				  g_sigs[WTERMSIG(status) - 1], swap);
	else
		ft_printf("\n[%d]    %d %s   %.*s...\n", i, process->pid,
				  g_sigs[WTERMSIG(status) - 1], 45, swap);
	ft_memdel((void **)&swap);
}

static void				close_foreign_fds(t_pipe_segment *processes,
											t_pipe_segment *current)
{
	struct s_fd_lst	*list;

	while (processes)
	{
		if (processes != current)
		{
			list = processes->
				context->
				fd_list;
			while (list)
			{
				if (list->current > 2)
					close(list->current);
				list = list->next;
			}
		}
		processes = processes->next;
	}
}

static void				write_heredocs(const t_io_rdr *io_rdrs)
{
	while (io_rdrs->type != TOKEN_NOT_APPLICABLE)
	{
		if ((io_rdrs->type == TOKEN_DLESS || io_rdrs->type == TOKEN_DLESSDASH))
		{
			if (g_term->fallback_input_state != STATE_NON_INTERACTIVE)
				write(io_rdrs->right.fd, io_rdrs->right.path,
					ft_strlen(io_rdrs->right.path));
			close(io_rdrs->right.fd);
		}
		else if (io_rdrs->type == TOKEN_TRILESS)
		{
			write(io_rdrs->right.fd, io_rdrs->right.path,
					ft_strlen(io_rdrs->right.path));
			write(io_rdrs->right.fd, "\n", 1);
			close(io_rdrs->right.fd);
		}
		io_rdrs++;
	}
}

static int				waitnclaim(t_pipe_segment *last)
{
	int			status;

	waitpid(last->pid, &status, 0);
	if (WIFSIGNALED(status))
		handle_signaled(last, status);
	tcsetpgrp(0, g_term->shell_pgid);
	last->status = status;
	return (WEXITSTATUS(last->status));
}

int forknrun(t_job_alt *job, t_pipe_segment *process, char *path, bool is_async)
{
	process->pid = fork();
	if (process->pid == 0)
	{
		job->pgid == 0 ? getpid() : job->pgid;
		setpgid(getpid(), job->pgid);
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTOU, SIG_IGN);
		context_switch(process->context);
		if (!is_async)
			tcsetpgrp(0, job->pgid);
		signal(SIGTTOU, SIG_DFL);
		close_redundant_fds(process->context);
		close_foreign_fds(job->pipeline, process);
//		environ_to_array_diff(process->context->environ, jc_get()->shell_context->environ,
//			SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL);
		execve(path, process->command->args, environ_to_array(process->context->environ,
			SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL));
		context_switch(jc_get()->shell_context);
		ft_dprintf(2, "42sh: execve error: %s\n", process->command->args[0]);
		exit(1);
	}
	else if (process->pid == -1)
		ft_dprintf(2, "42sh: fork error: %s\n", process->command->args[0]);
	else
	{
		if (job->pgid == 0)
			job->pgid = process->pid;
		setpgid(process->pid, job->pgid);
		write_heredocs(process->command->io_redirects);
		close_redundant_fds(process->context);
		environ_deallocate_vector(process->context->environ);
		process->context->environ = NULL;
//		context_deep_free(&process->context);
		if (process->next == NULL && !is_async)
			return (waitnclaim(process));
	}
	return (-256);
}
