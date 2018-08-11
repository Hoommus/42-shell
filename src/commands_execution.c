/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:46:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/twenty_one_sh.h"

int		try_builtin(char *builtin, char **args)
{
	int		i;

	i = 0;
	while (i < 10 && builtin)
	{
		if (ft_strcmp(builtin, g_builtins[i]) == 0)
			return (g_builtin_func[i](args));
		i++;
	}
	return (1);
}

int		forknrun(char *bin, char **args)
{
	int		status;

	g_running_process = fork();
	if (g_running_process == 0)
	{
		execve(bin, args, g_environ);
		exit(0);
	}
	else
	{
		wait(&status);
		g_running_process = 0;
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
	if (swap == NULL)
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
	free_array(paths);
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
	int		status;

	status = 1;
	if (args != NULL)
		status = try_builtin(args[0], args + 1);
	if (status == 0)
		return (0);
	status = try_binary(args[0], args);
	if (status == 0)
		return (0);
	status = try_local_binary(args[0], args);
	if (status == 0)
		return (0);
	return (1);
}
