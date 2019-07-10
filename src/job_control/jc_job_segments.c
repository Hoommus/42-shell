/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_job_segments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 14:43:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 17:41:17 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_builtins.h"

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

static int		forknrun_builtin(t_proc *segment, int (*bltin)(const char **))
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
				close_redundant_fds(segment->context);
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
		close_foreign_fds(job->procs, (t_proc *)list);
		list = list->next;
	}
	jc_disable_subshell();
	if (jc_is_subshell() || !is_async)
		return (jc_wait(job));
	return (-1024);
}
