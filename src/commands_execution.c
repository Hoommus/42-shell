#include "../include/minishell.h"

int		try_builtin(char *builtin, char **args)
{
	int		i;

	i = 0;
	while (i < 10)
	{
		if (ft_strcmp(builtin, g_builtins[i]) == 0)
			return (g_builtin_func[i](args));
		i++;
	}
	return (1);
}

int		forknrun(char *bare_bin, char *bin, char **args)
{
	pid_t	pid;
	int		status;

	*bare_bin = bare_bin[0];
	pid = fork();
	if (pid == 0)
	{
		execve(bin, args, g_environ);
		exit(0);
	}
	else
	{
		pid = wait(&status);
		return (0);
	}
	return (1);
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
	free(swap);
	i = 0;
	status = 1;
	while (paths[i] && status)
	{
		swap = ft_strings_join(2, "/", paths[i], binary, NULL);
		if (access(swap, X_OK) == 0)
			if (!forknrun(binary, swap, args))
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

	status = 1;
	if (access(bin, X_OK) == 0 && !forknrun(bin, bin, args))
		status = 0;
	return (status);
}

int		execute(char **args)
{
	int		status;

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
