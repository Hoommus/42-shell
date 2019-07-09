/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_job_launcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 04:03:21 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 04:07:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include "shell_job_control.h"
#include "shell_builtins.h"

void			jc_job_dealloc(t_job **job)
{
	process_list_destroy(&(*job)->procs);
	free((*job)->command);
	free(*job);
	*job = NULL;
}

t_job			*jc_tmp_add(t_proc *segment)
{
	struct s_job_control	*jc;

	jc = jc_get();
	if (jc->tmp_job == NULL)
		jc->tmp_job = ft_memalloc(sizeof(t_job));
	process_list_add(&jc->tmp_job->procs, segment);
	return (jc->tmp_job);
}

static char		*path_to_target(t_proc *segment)
{
	const char	**args = (const char **)segment->command->args;
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

static int		forknrun_builtin(t_proc *segment,
	int (*bltin)(const char **))
{
	int						status;

	if ((status = fork()) == 0)
	{
		context_switch(segment->context);
		exit(bltin((const char **)segment->command->args + 1));
	}
	else if (status == -1)
		ft_dprintf(2, SH ": fork for builtin %s failed\n",
			segment->command->args[0]);
	else
		context_deep_free(&segment->context);
	return (0);
}

static int		run_builtin(t_proc *segment, bool is_async)
{
	extern struct s_builtin	g_builtins[];
	int						i;
	const char				*bltin;

	bltin = segment->command->args ? segment->command->args[0] : NULL;
	i = -1;
	while (bltin && g_builtins[++i].name)
		if (ft_strcmp(bltin, g_builtins[i].name) == 0)
		{
			if (!is_async)
			{
				context_switch(segment->context);
				segment->status = g_builtins[i].
					function((const char **)segment->command->args + 1);
				context_switch(jc_get()->shell_context);
				return (segment->status);
			}
			else
				return (forknrun_builtin(segment, g_builtins[i].function));
		}
	return (-512);
}

static int		run_regular(t_job *job, t_proc *process, bool is_async)
{
	int				status;
	char			*bin;

	status = -256;
	if ((bin = path_to_target(process)) != NULL
		&& access(bin, F_OK) == -1 && ft_strchr(bin, '/') != NULL)
		ft_dprintf(2, ERR_NO_SUCH_FILE, process->command->args[0]);
	else if (bin != NULL && is_dir(bin))
		ft_dprintf(2, ERR_IS_A_DIRECTORY, process->command->args[0]);
	else if (bin != NULL && access(bin, X_OK) == -1)
		ft_dprintf(2, ERR_PERMISSION_DENIED, process->command->args[0]);
	else if (bin == NULL)
		ft_dprintf(2, ERR_COMMAND_NOT_FOUND, process->command->args[0]);
	else if (!g_interrupt)
		status = forknrun(job, process, bin, is_async);
	ft_memdel((void **)&bin);
	return (status);
}

int				execute_segments(t_job *job, bool is_async)
{
	const t_proc	*list = job->procs;
	struct termios	termios;
	int				s;

	s = -1024;
	if ((list && list->next) || is_async)
		jc_enable_subshell();
	while (list)
	{
		if ((s = run_builtin((t_proc *)list, is_async)) != -512 && !list->next)
			return (s);
		else if (s == -512)
			run_regular(job, (t_proc *)list, is_async);
		tcgetattr(0, &termios);
		*list->context->term_config = termios;
		close_redundant_fds(list->context);
		list = list->next;
	}
	jc_disable_subshell();
	if (jc_is_subshell() || !is_async)
		return (jc_wait(job));
	return (-1024);
}

int				jc_wait(t_job *job)
{
	t_proc		*procs;
	int			status;

	poll_pipeline(job, false);
	procs = job->procs;
	while (procs->next)
		procs = procs->next;
	alterate_proc(job, procs);
	status = procs->status;
	if (job->state == JOB_SIGTSTP || job->state == JOB_SIGSTOP ||
		job->state == JOB_SIGTTIN || job->state == JOB_SIGTTOU)
		return (-1024);
	return (status);
}

int				jc_resolve_status(t_job *job)
{
	t_proc		*proc;

	proc = job->procs;
	while (proc->next)
		proc = proc->next;
	if (proc->pid != 0 && WIFSIGNALED(proc->status))
	{
		job->state = WTERMSIG(proc->status) + 30;
		ft_dprintf(2, "%s %s\n", jc_state_str(job->state), job->command);
		return (WTERMSIG(proc->status));
	}
	else if (proc->pid != 0 && WIFSTOPPED(proc->status))
	{
		job->state = WSTOPSIG(proc->status) > 0
					? WSTOPSIG(proc->status) + 30 : JOB_STOPPED;
		return (WSTOPSIG(proc->status));
	}
	else if (proc->pid != 0 && WIFEXITED(proc->status))
	{
		job->state = JOB_TERMINATED;
		return (WEXITSTATUS(proc->status));
	}
	return (-2048);
}

int				jc_launch(t_job *job, bool is_async)
{
	int				status;

	if (job->procs && job->procs->command)
		job->command = ft_strarr_join(" ", job->procs->command->args);
	status = execute_segments(job, is_async);
	if (!is_async)
	{
		tcsetpgrp(0, g_term->shell_pgid);
		status = jc_resolve_status(job);
		status = status == -2048 ? 127 : status;
	}
	tcsetattr(0, TCSADRAIN, g_term->shell_term);
	if (!jc_is_subshell() && (is_async || status == -1024))
	{
		job->state = JOB_LAUNCHED;
		jc_register_job(job);
		job->state = JOB_RUNNING;
	}
	else if (job->state == JOB_SIGTSTP || job->state == JOB_SIGSTOP ||
			job->state == JOB_SIGTTIN || job->state == JOB_SIGTTOU)
		jc_register_job(job);
	else
		jc_job_dealloc(&(job));
	return (status);
}

int				jc_tmp_finalize(bool is_async)
{
	t_job		*job;
	int			status;

	job = jc_get()->tmp_job;
	if (job == NULL)
		return (0);
	jc_get()->tmp_job = NULL;
	status = jc_launch(job, is_async);
	return (status);
}
