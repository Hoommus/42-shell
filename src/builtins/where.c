/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   where.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/01 12:51:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/19 14:02:25 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

char	*is_builtin(char *arg)
{
	extern struct s_builtin	g_builtins[];
	int						i;

	if (arg == NULL)
		return (NULL);
	i = 0;
	while (g_builtins[i].name != NULL)
	{
		if (ft_strcmp(arg, g_builtins[i].name) == 0)
			return (g_builtins[i].name);
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
