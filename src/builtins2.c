#include "../include/minishell.h"

int				hs_env(char **args)
{
	int			i;

	i = 0;
	while (g_environ[i])
		ft_printf("%s\n", g_environ[i++]);
	*args = args[0];
	return (0);
}

int				hs_setenv(char **args)
{
	int		len;
	char	**swap;

	len = 0;
	while (args[len])
		len++;
	if (len > 3 || len == 0)
		ft_printf("usage:\nsetenv [KEY=VALUE | KEY = VALUE | KEY VALUE]\n");
	else if (len == 2)
		set_env(args[0], args[1]);
	else if (len == 3)
		set_env(args[0], args[2]);
	else
	{
		swap = ft_strsplit(args[0], '=');
		set_env(swap[0], swap[1]);
		free_array(swap);
	}
	return (0);
}

int				hs_unsetenv(char **args)
{
	int		len;

	len = 0;
	while (args[len])
		len++;
	if (len > 1 || len == 0)
		ft_printf("usage:\nunsetenv KEY [KEY ...]\n");
	else
	{
		if (ft_strcmp(args[0], "all") == 0)
		{
			len = 0;
			while (g_environ[len])
				free(g_environ[len++]);
		}
		else
			unset_env(args[0]);
	}
	return (0);
}

int				hs_where(char **args)
{
	char	*swap;
	char	**paths;

	swap = get_env("PATH");
	if (swap == NULL)
		return (1);
	paths = ft_strsplit(swap, ':');
	free(swap);
	while (*args)
		hs_where_auxilia(paths, *args++);
	free_array(paths);
	return (0);
}
