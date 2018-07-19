#include "../include/minishell.h"

int		try_builtin(char *builtin, char **args)
{
	int		i;

	i = 0;
	while (i < 8)
	{
		if (ft_strcmp(builtin, g_builtins[i]) == 0)
			return (g_builtin_func[i](args));
		i++;
	}
	return (1);
}

int		forknrun(char *bare_bin, char *bin, char **args, char **environ)
{
	pid_t	pid;

	*bare_bin = bare_bin[0];
	pid = fork();
	if (pid == 0)
	{
		execve(bin, args, environ);
		exit(0);
	}
	else
	{
		wait(&pid);
		return (0);
	}
	return (1);
}

int		try_binary(char *binary, char **args, char **environ)
{
	int		i;
	char	*swap;
	char	**paths;

	swap = get_env("PATH", environ);
	paths = ft_strsplit(swap, ':');
	free(swap);
	i = 0;
	while (paths[i])
	{
		swap = ft_strings_join("/", paths[i], binary, NULL);
		if (access(swap, X_OK) == 0)
		{
			if (!forknrun(binary, swap, args, environ))
			{
				free(swap);
				return (0);
			}
		}
		free(swap);
		i++;
	}
	return (1);
}

int		execute(char **args, char **environ)
{
	int		status;

	status = try_builtin(args[0], args + 1);
	if (status == 0)
		return (0);
	status = try_binary(args[0], args + 1, environ);
	if (status == 0)
		return (0);
	return (1);
}
