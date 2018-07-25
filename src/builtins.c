#include "../include/minishell.h"

int				hs_echo(char **args)
{
	char	*str;

	str = ft_strarr_join(" ", args);
	replace_variables(str);
	if (str == 0)
		return (1);
	ft_printf("%s\n", str);
	free(str);
	return (0);
}

int				hs_cd(char **args)
{
	char		*home;

	home = get_env("HOME");
	if (args == NULL)
		return (1);
	if (args[0] != NULL && ft_strcmp(args[0], "~") != 0)
		chdir(args[0]);
	else if (ft_strcmp(args[0], "-") == 0)
		chdir(args[0]);
	else if (home != NULL && args[0] != NULL && ft_strcmp(args[0], "~") == 0)
		chdir(home);
	else if (home != NULL)
		chdir(home);
	if (home != NULL)
		free(home);
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
