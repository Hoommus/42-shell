/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 21:57:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/08 23:49:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_builtins.h"
#include "twenty_one_sh.h"

t_env_vector	*g_alias_vector;

static void		push_name_value(const char *arg)
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
			environ_push_entry(g_alias_vector, alias, arg + i + 1,
				SCOPE_SHELL_LOCAL);
			free(alias);
			return ;
		}
		i++;
	}
}

char			*alias_get(const char *name)
{
	t_var		*entry;

	if (!g_alias_vector)
		return (NULL);
	entry = environ_get_entry(g_alias_vector, name);
	if (entry)
		return (entry->value);
	return (NULL);
}

int				alias_remove(const char *name)
{
	if (!g_alias_vector || !environ_get_entry(g_alias_vector, name))
		return (1);
	environ_remove_entry(g_alias_vector, name);
	return (0);
}

int				alias_remove_all(void)
{
	const t_var	*array = (t_var *)g_alias_vector->array;
	u_int32_t	i;

	i = 0;
	while (i < g_alias_vector->size)
	{
		environ_remove_entry(g_alias_vector, array[0].key);
		i++;
	}
	return (0);
}

int				hs_alias(const char **args)
{
	u_int32_t	i;
	t_var		*entry;
	int			status;

	status = 0;
	if (args == NULL || args[0] == NULL)
	{
		print_var_vector(g_alias_vector, SCOPE_SHELL_LOCAL);
		return (1);
	}
	i = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			push_name_value(args[i]);
		else if ((entry = environ_get_entry(g_alias_vector, args[i])))
			print_var(entry);
		else
			status = 1;
		i++;
	}
	return (status);
}
