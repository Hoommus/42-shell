/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilia.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:45:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			is_valid_var(char *var)
{
	int		i;

	if (var == NULL)
		return (0);
	i = 0;
	while (var[i])
	{
		if (!(ft_isalnum(var[i]) || var[i] == '_' || var[i] == '$'))
			return (0);
		i++;
	}
	return (1);
}

char		*is_builtin(char *arg)
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

void		hs_where_auxilia(char **paths, char *arg)
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
		ft_printf("%s: not found\n", arg);
}

void		restore_variables(void)
{
	char	*var;

	if ((var = get_env("PWD")) == NULL)
		set_env("PWD", " ");
	chfree(var);
	if ((var = get_env("HOME")) == NULL)
		set_env("HOME", " ");
	chfree(var);
	if ((var = get_env("OLDPWD")) == NULL)
		set_env("OLDPWD", (var = get_env("HOME")));
	chfree(var);
	if ((var = get_env("PATH")) == NULL)
		set_env("PATH", " ");
	chfree(var);
}
