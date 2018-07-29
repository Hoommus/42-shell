#include "../include/minishell.h"

int				hs_echo(char **args)
{
	char	*str;
	char	*swap;

	str = ft_strarr_join(" ", args);
	swap = replace_variables(str);
	free(str);
	if (swap == NULL)
		return (1);
	ft_printf("%s\n", swap);
	free(swap);
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
	while (i < 9)
		ft_printf("%s, ", g_builtins[i++]);
	ft_printf("%s\n", g_builtins[9]);
	return (0);
}

int				hs_exit(char **args)
{
	*args = args[0];
	exit(0);
	return (0);
}
