/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 16:42:51 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/17 17:06:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_builtins.h"

static char					*g_sigs[31] =
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

static void					close_pipe_fds(t_context *context)
{
	struct s_fd_lst		*list;

	list = context->fd_list;
	while (list)
	{
		if (ft_strcmp(list->label, "pipe") == 0)
			close(list->current);
		list = list->next;
	}
}

static char					*path_to_target(t_job *job)
{
	const char	**args = (const char **)job->args;
	t_var		*var;
	int			i;
	char		*swap;
	char		**paths;

	if (ft_strchr(args[0], '/') != NULL)
		return (ft_strdup(args[0]));
	var = get_env_v(NULL, "PATH");
	if (!var || !var->value)
		return (NULL);
	paths = ft_strsplit(var->value, ':');
	i = 0;
	swap = NULL;
	while (paths[i])
	{
		swap = ft_strings_join(2, "/", paths[i], (char *)args[0], NULL);
		if (access(swap, F_OK) == 0)
			break ;
		ft_memdel((void **)&swap);
		i++;
	}
	free_array((void **)paths);
	return (swap);
}

static int					run_builtin(t_job *job)
{
	extern struct s_builtin	g_builtins[];
	const char				*builtin = job->args ? job->args[0] : NULL;
	int						i;

	i = 0;
	while (builtin && g_builtins[i].name)
	{
		if (ft_strcmp(builtin, g_builtins[i].name) == 0)
		{
			context_switch(job->context);
			job->exit_status = g_builtins[i].function((const char **)job->args + 1);
			close_pipe_fds(job->context);
			context_switch(jc_get()->shell_context);
			return (job->exit_status);
		}
		i++;
	}
	return (-512);
}

static void					handle_signaled(t_job *job, int status)
{
	char		*swap;
	t_job		*list;
	int			i;

	i = 1;
	list = jc_get()->jobs;
	while (list && ++i)
		list = list->next;
	swap = ft_strarr_join(" ", job->args);
	if (ft_strlen(swap) < 45)
		ft_printf("\n[%d]    %d %s   %s\n", i, job->pid,
			g_sigs[WTERMSIG(status) - 1], swap);
	else
		ft_printf("\n[%d]    %d %s   %.*s...\n", i, job->pid,
			g_sigs[WTERMSIG(status) - 1], 45, swap);
	ft_memdel((void **)&swap);
}

void						sigpipe_kill_left(t_job *pivot)
{
	int					status;

	pivot = pivot->prev;
	ft_printf("%p\n", pivot);
	while (pivot)
	{
		kill(pivot->pid, SIGPIPE);
		waitpid(pivot->pid, &status, 0);
		pivot->exit_status = WEXITSTATUS(status);
		pivot->state = JOB_TERMINATED;
		pivot = pivot->prev;
	}
}

static void					waitnclaim(t_job *job)
{
	int			status;
	int			wait_status;

	wait_status = waitpid(job->pid, &status, 0);
	tcsetpgrp(0, jc_get()->shell_pid);
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
//	ft_printf("wait_status = %d\nexit_status = %d\nWIFSIGNALED = %s ; "
//				"WIFEXITED = %s\nWTERMSIG = %d\n",
//				wait_status, job->exit_status,
//				WIFSIGNALED(status) ? "true" : "false",
//				WIFEXITED(status) ? "true" : "false", WTERMSIG(status));
	if (job->exit_status == 0
		&& WIFSIGNALED(status) && !WIFEXITED(status)
		&& WTERMSIG(status) != 1 && WTERMSIG(status) <= 31)
		handle_signaled(job, status);
	if (wait_status > 0)
		g_term->last_status = WEXITSTATUS(status);
	jc_collect_zombies();
}
// TODO: think about what happens if queue has multiple FG jobs even though it shouldn't
static int					forknrun(t_job *job, char *path)
{
	job->pid = fork();
	if (job->pid == 0)
	{
		context_switch(job->context);
		execve(path, job->args, environ_to_array(job->context->environ,
			SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL));
		context_switch(jc_get()->shell_context);
		ft_dprintf(2, "21sh: execve error: %s\n", job->args[0]);
	}
	else if (job->pid == -1)
		ft_dprintf(2, "21sh: fork error: %s\n", job->args[0]);
	else
	{
		close_pipe_fds(job->context);
		if (job->next == NULL)
		{
			waitnclaim(job);
			ft_memdel((void **)&path);
			return (g_term->last_status);
		}
//		else
//			jc_register_job(job);
		ft_memdel((void **)&path);
	}
	return (-1024); // error indicator
}

int							jc_execute_pipeline_queue(void)
{
	int			status;
	t_job		*list;
	char		*bin;

	list = jc_get()->job_queue;
	while (list)
	{
		if ((g_term->last_status = run_builtin(list)) != -512)
		{
			if (!list->next)
				return (g_term->last_status);
		}
		else
		{
			bin = path_to_target(list);
			if (bin != NULL && access(bin, X_OK) == -1)
				ft_dprintf(2, "21sh: permission denied: %s\n", list->args[0]);
			else if (bin == NULL)
				ft_dprintf(2, "21sh: command not found: %s\n", list->args[0]);
			else if (!g_interrupt && (status = forknrun(list, bin)) != -1024)
				return (status);
			if (bin == NULL || access(bin, X_OK) == -1)
				return (-1024);
		}
		list = list->next;
	}
	return (-1024); // error
}
