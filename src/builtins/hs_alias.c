
#include "shell_builtins.h"
#include "twenty_one_sh.h"

static t_env_vector	*g_alias_vector;

static void			push_name_value(const char *arg)
{
	size_t		len;
	size_t		i;
	char		*alias;

	i = 0;
	len = ft_strlen(arg);
	while (i < len)
	{
		if (arg[i] == '=')
		{
			alias = ft_strsub(arg, 0, i);
			environ_push_entry(g_alias_vector, alias, arg + i + 1, SCOPE_GLOBAL);
			free(alias);
			return ;
		}
		i++;
	}
}

char			*get_alias(const char *name)
{
	t_var		*entry;

	if (!g_alias_vector)
		return (NULL);
	entry = environ_get_entry(g_alias_vector, name);
	if (entry)
		return (entry->value);
	return (NULL);
}

int				hs_alias(const char **args)
{
	u_int32_t	i;
	t_var		*entry;
	int			status;

	status = 0;
	if (g_alias_vector == NULL)
		g_alias_vector = environ_create_vector(8);
	if (args == NULL || args[0] == NULL)
		return (1);
	i = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '=') == NULL)
			push_name_value(args[i]);
		else if ((entry = environ_get_entry(g_alias_vector, args[i])))
			print_var(entry);
		else
			status = 1;
		i++;
	}
	return (status);
}
