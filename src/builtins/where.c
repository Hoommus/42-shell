/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   where.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/01 12:51:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/08/01 13:02:49 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/twenty_one_sh.h"

char	*is_builtin(char *arg)
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

void	hs_where_auxilia(char **paths, char *arg)
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
		chfree(swap);
	}
	if (is_builtin(arg))
		ft_printf("%s: shell built-in command\n", arg);
	if (where)
	{
		ft_printf("%s\n", where);
		chfree(where);
	}
	else if (!is_builtin(arg))
		ft_dprintf(2, "where: %s: not found\n", arg);
}

/*
** seems like some code is shared with try_binary
*/

int		hs_where(char **args)
{
	char	*swap;
	char	**paths;

	swap = get_env("PATH");
	paths = ft_strsplit(swap, ':');
	while (*args)
		hs_where_auxilia(paths, *args++);
	free_array(paths);
	return (0);
}
