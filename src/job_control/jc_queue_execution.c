/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 16:42:51 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 19:46:28 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_builtins.h"

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
		if (access(swap, X_OK) == 0)
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
			context_switch(NULL);
			return (job->exit_status);
		}
		i++;
	}
	return (-127);
}

// TODO: think about what happens if command does not exist
int							jc_execute_queue(void)
{
	int			status;
	t_job		*list;
	pid_t		fork_pid;
	char		*binary;

	list = jc_get()->job_queue;
	while (list)
	{
		if (run_builtin(list) != -127)
		{
			list = list->next;
			continue ;
		}
		binary = path_to_target(list);
		if (binary != NULL)
		{
			fork_pid = fork();
			if (fork_pid == 0)
			{
				context_switch(list->context);
				execve(binary, list->args,
					environ_to_array(list->context->environ,
					SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL));
				exit(0);
			}
			else
			{
				list->pid = fork_pid;
				if (list->state == JOB_FG)
				{
					waitpid(fork_pid, &status, 0);
					tcsetpgrp(g_term->tty_fd, jc_get()->shell_pid);
					TERM_APPLY_CONFIG(g_term->context_current->term_config);
					ft_memdel((void **)&binary);
					// TODO: think what happens if queue has multiple FG jobs
					return (g_term->last_cmd_status = WEXITSTATUS(status));
				}
				else
					jc_register_job(list);
			}
		}
		else
			ft_dprintf(2, "21sh: jc: command not found: %s\n", list->args[0]);
		ft_memdel((void **)&binary);
		list = list->next;
	}
	return (-1024); // Why did I do this?..
}
