/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_forknrun.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/25 18:31:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/25 12:14:39 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "shell_job_control.h"
#include "shell_builtins.h"

char					*g_sigs[] =
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

void					handle_signaled(t_job *job, int status)
{
	if (ft_strlen(job->command) < 45)
		ft_printf("\n        %d %s   %s\n", job->pgid,
			g_sigs[WTERMSIG(status) - 1], job->command);
	else
		ft_printf("\n        %d %s   %.*s...\n", job->pgid,
			g_sigs[WTERMSIG(status) - 1], 45, job->command);
}

static void				close_foreign_fds(t_proc *processes,
											t_proc *current)
{
	struct s_fd_lst	*list;

	while (processes)
	{
		if (processes != current)
		{
			list = processes->context->
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

static void	prepare_exec(t_job *job, t_proc *process, bool is_async)
{
	pid_t	pgid;

	pgid = job->pgid == 0 ? getpid() : job->pgid;
	if (is_async)
		setpgid(getpid(), pgid);
	//reset_signal_handlers();
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGTTIN, SIG_DFL);
	signal(SIGWINCH, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
	signal(SIGTTOU, SIG_IGN);
	context_switch(process->context);
	if (!is_async && tcgetpgrp(0) != pgid)
		tcsetpgrp(0, pgid);
	signal(SIGTTOU, SIG_DFL);
	close_redundant_fds(process->context);
	close_foreign_fds(job->procs, process);
}

int forknrun(t_job *job, t_proc *process, char *path, bool is_async)
{
	process->pid = fork();
	if (process->pid == 0)
	{
		prepare_exec(job, process, is_async);
		execve(path, process->command->args,
			environ_to_array_diff(process->context->environ,
				jc_get()->shell_context->environ,
				SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL));
		context_switch(jc_get()->shell_context);
		ft_dprintf(2, SH ": execve error: %s\n", process->command->args[0]);
		exit(1);
	}
	else if (process->pid == -1)
		ft_dprintf(2, SH ": fork error: %s\n", process->command->args[0]);
	else
	{
		if (job->pgid == 0)
			job->pgid = process->pid;
		setpgid(process->pid, is_async ? job->pgid : g_term->shell_pgid);
		if (!is_async)
			tcsetpgrp(0, job->pgid);
		write_heredocs(process->command->io_redirects);
		close_redundant_fds(process->context);
		environ_deallocate_vector(process->context->environ);
		process->context->environ = NULL;
	}
	return (-256);
}
