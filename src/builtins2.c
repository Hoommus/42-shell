#include "../include/minishell.h"

int				hs_env(char **args)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
		ft_printf("%s\n", environ[i++]);
	*args = args[0];
	return (0);
}

int				hs_setenv(char **args)
{
	*args = args[0];

	return (0);
}

int				hs_unsetenv(char **args)
{
	*args = args[0];

	return (0);
}
