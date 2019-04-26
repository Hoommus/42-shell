/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_queue_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 16:42:51 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:34:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_builtins.h"

static char				*path_to_target(t_job *job)
{
	const char	**args = (const char **)job->cmd->args;
	t_var		*var;
	int			i;
	char		*swap;
	char		**paths;

	if (ft_strchr(args[0], '/') != NULL)
		return (access(args[0], F_OK) == -1 ? NULL : ft_strdup(args[0]));
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

static int				run_builtin(t_job *job)
{
	extern struct s_builtin	g_builtins[];
	const char				*bltin = job->cmd->args ? job->cmd->args[0] : NULL;
	int						i;

	i = 0;
	while (bltin && g_builtins[i].name)
	{
		if (ft_strcmp(bltin, g_builtins[i].name) == 0)
		{
			context_switch(job->context);
			job->exit_status = g_builtins[i].
				function((const char **)job->cmd->args + 1);
			close_redundant_fds(job->context);
			context_switch(jc_get()->shell_context);
			return (job->exit_status);
		}
		i++;
	}
	return (-512);
}

#define DIRTY_HACK(err) (ft_dprintf(2, err, list->cmd->args[0]) & 0) & -1024

int						jc_execute_pipeline_queue(void)
{
	int			status;
	t_job		*list;
	char		*bin;

	list = jc_get()->job_queue;
	while (list)
	{
		if ((g_term->last_status = run_builtin(list)) != -512 && !list->next)
			return (g_term->last_status);
		else if (g_term->last_status == -512)
		{
			if ((bin = path_to_target(list)) != NULL && access(bin, F_OK) == -1)
				return (DIRTY_HACK(ERR_NO_SUCH_FILE));
			else if (bin != NULL && is_dir(bin))
				return (DIRTY_HACK(ERR_IS_A_DIRECTORY));
			else if (bin != NULL && access(bin, X_OK) == -1)
				return (DIRTY_HACK(ERR_PERMISSION_DENIED));
			else if (bin == NULL)
				return (DIRTY_HACK(ERR_COMMAND_NOT_FOUND));
			else if (!g_interrupt && (status = forknrun(list, bin)) != -1024)
				return (status);
		}
		list = list->next;
	}
	return (-1024);
}
