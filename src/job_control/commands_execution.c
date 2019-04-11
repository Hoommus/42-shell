/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 15:26:38 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"
#include "shell_builtins.h"

int		try_builtin(const char *builtin, const char **args)
{
	extern struct s_builtin	g_builtins[];
	int						i;

	i = 0;
	while (builtin && g_builtins[i].name)
	{
		if (ft_strcmp(builtin, g_builtins[i].name) == 0)
			return (g_builtins[i].function(args));
		i++;
	}
	return (1);
}

int		forknrun(const char *bin, const char **args, bool is_async)
{
	int		status;
	char	**environ;
	pid_t	pid;

	environ = NULL;
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	pid = fork();
	if (pid == 0)
	{
		environ = environ_to_array(g_term->context_current->environ,
			SCOPE_EXPORT | SCOPE_COMMAND_LOCAL | SCOPE_SCRIPT_GLOBAL);
		execve(bin, (char **)args, environ);
		free_array((void **)environ);
		exit(0);
	}
	else
	{
		if (is_async)
			ft_printf("Waiting for process %d...\n", pid);
		else
			ft_printf("Launched %d and passing control\n", pid);
		//add_job(pid, g_term->context_current, is_async);
		//waitpid(pid, &status, is_async ? WNOHANG : 0);
		ft_printf("Finished waiting for %d, status %d\n", pid, WEXITSTATUS(status));
		// TODO: Do something with these lines
		  if (!is_async)
		  	g_term->last_cmd_status = WEXITSTATUS(status);
		TERM_APPLY_CONFIG(g_term->context_current->term_config);
		free_array((void **)environ);
		return (0);
	}
}

int		try_binary(const char *binary, const char **args, bool does_wait)
{
	t_var	*var;
	int		i;
	int		status;
	char	*swap;
	char	**paths;

	var = get_env_v(NULL, "PATH");
	if (!var || !var->value || (binary && ft_strchr(binary, '/') != NULL))
		return (1);
	paths = ft_strsplit(var->value, ':');
	i = 0;
	status = 1;
	while (paths[i] && status)
	{
		swap = ft_strings_join(2, "/", paths[i], binary, NULL);
		if (access(swap, X_OK) == 0)
			if (!forknrun(swap, args, does_wait))
				status = 0;
		free(swap);
		i++;
	}
	free_array((void **)paths);
	return (status);
}

int		try_local_binary(const char *bin, const char **args, bool does_wait)
{
	int		status;

	status = 1;
	if (access(bin, X_OK) == 0 && !forknrun(bin, args, does_wait))
		status = 0;
	return (status);
}

int		execute(const char **args, const bool does_wait)
{
	g_term->last_cmd_status = 1;
	if (args== NULL)
		return (1);
	g_term->last_cmd_status = try_builtin(args[0], args + 1);
	if (g_term->last_cmd_status != 0)
		g_term->last_cmd_status = try_binary(args[0], args, does_wait);
	if (g_term->last_cmd_status != 0)
		g_term->last_cmd_status = try_local_binary(args[0], args, does_wait);
	return (g_term->last_cmd_status);
}
