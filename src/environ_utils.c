#include "../include/minishell.h"

char	*get_env(char *name, char **env)
{
	int		i;
	char	**swap;
	char	*dummy;

	i = 0;
	while (env[i])
	{
		swap = ft_strsplit(env[i], '=');
		if (ft_strcmp(swap[0], name) == 0)
		{
			dummy = ft_strdup(swap[1]);
			free(swap[0]);
			free(swap[1]);
			free(swap);
			return (dummy);
		}

		i++;
	}
	return (NULL);
}

