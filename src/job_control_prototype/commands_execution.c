/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/18 14:06:10 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

int		try_builtin(char *builtin, char **args)
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

int		forknrun(char *bin, char **args)
{
	int		status;

	TERM_RESTORE;
	g_term->running_process = fork();
	if (g_term->running_process == 0)
	{
		execve(bin, args, g_environ);
		exit(0);
	}
	else
	{
		//TODO: Replace with waitpid() with WUNTRACED to make Ctrl+Z work
		//waitpid(g_term->running_process, &status, WNOHANG | WUNTRACED);
		wait(&status);
		g_term->last_cmd_status = WEXITSTATUS(status);
		g_term->running_process = 0;
		TERM_ENFORCE;
		return (0);
	}
}

int		try_binary(char *binary, char **args)
{
	int		i;
	int		status;
	char	*swap;
	char	**paths;

	swap = get_env("PATH");
	if (swap == NULL || (binary && ft_strchr(binary, '/') != NULL))
		return (1);
	paths = ft_strsplit(swap, ':');
	i = 0;
	status = 1;
	while (paths[i] && status)
	{
		swap = ft_strings_join(2, "/", paths[i], binary, NULL);
		if (access(swap, X_OK) == 0)
			if (!forknrun(swap, args))
				status = 0;
		free(swap);
		i++;
	}
	free_array((void **)paths);
	return (status);
}

int		try_local_binary(char *bin, char **args)
{
	int		status;
	char	*swap;

	status = 1;
	swap = replace_home(bin);
	if (access(swap, X_OK) == 0 && !forknrun(swap, args))
		status = 0;
	chfree(swap);
	return (status);
}

int		execute(char **args)
{
	g_term->last_cmd_status = 1;
	if (args != NULL)
		g_term->last_cmd_status = try_builtin(args[0], args + 1);
	if (g_term->last_cmd_status == 0)
		return (0);
	g_term->last_cmd_status = try_binary(args[0], args);
	if (g_term->last_cmd_status == 0)
		return (0);
	g_term->last_cmd_status = try_local_binary(args[0], args);
	if (g_term->last_cmd_status == 0)
		return (0);
	return (1);
}
