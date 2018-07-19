#include "../include/minishell.h"

int				hs_echo(char **args)
{
	char	*str;

	ft_printf("Echoing...\n");
	str = ft_strarr_join(" ", args);
	if (str == 0)
		return (1);
	ft_printf("%s\n", str);
	free(str);
	return (0);
}

int				hs_cd(char **args)
{
	*args = args[0];

	return (0);
}

int				hs_alias(char **args)
{
	*args = args[0];

	return (0);
}

int				hs_help(char **args)
{
	int		i;

	*args = args[0];
	ft_printf("Existing builtins:\n");
	i = 0;
	while (i < 7)
		ft_printf("%s, ", g_builtins[i++]);
	ft_printf("%s\n", g_builtins[7]);
	return (0);
}

int				hs_exit(char **args)
{
	*args = args[0];
	exit(0);
	return (0);
}
