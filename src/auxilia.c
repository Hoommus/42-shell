#include "../include/minishell.h"

char			*is_builtin(char *arg)
{
	int		i;

	if (arg == NULL)
		return (NULL);
	i = 0;
	while (i < 10)
	{
		if (ft_strcmp(arg, g_builtins[i]) == 0)
			return (g_builtins[i]);
		i++;
	}
	return (NULL);
}

void			hs_where_auxilia(char **paths, char *arg)
{
	int		i;
	char	*swap;
	char	*where;

	where = NULL;
	i = -1;
	while (paths[++i])
	{
		swap = ft_strings_join(2, "/", paths[i], arg, NULL);
		if (access(swap, X_OK) == 0)
			where = ft_strdup(swap);
		free(swap);
	}
	if (is_builtin(arg))
		ft_printf("%s: shell built-in command\n", arg);
	if (where)
	{
		ft_printf("%s\n", where);
		free(where);
	}
	else if (!is_builtin(arg))
		ft_printf("%s not found\n", arg);
}
