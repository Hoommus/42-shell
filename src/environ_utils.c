/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:54 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/08/01 12:27:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_env(char *name)
{
	int		i;
	char	**swap;
	char	*dummy;

	if (!is_valid_var(name))
	{
		ft_printf("env: not valid in this context: %s\n", name);
		return (0);
	}
	dummy = NULL;
	i = 0;
	while (g_environ[i])
	{
		if (ABS(ft_strcmp(name, g_environ[i])) == '=')
		{
			swap = ft_strsplit(g_environ[i], '=');
			dummy = swap[1] == NULL ? ft_strnew(0) : ft_strdup(swap[1]);
			free_array(swap);
			break ;
		}
		i++;
	}
	return (dummy);
}

int		set_env(char *key, char *value)
{
	int		i;
	char	**swap;
	char	*dummy;

	i = -1;
	if ((dummy = get_env(key)) == NULL)
	{
		while (g_environ[++i])
			;
		swap = (char **)ft_memalloc(sizeof(char *) * (i + 2));
		ft_memcpy(swap, g_environ, sizeof(char *) * (i));
		swap[i] = ft_strings_join(2, "=", key, value);
		free(g_environ);
		g_environ = swap;
	}
	while (dummy && g_environ[++i])
		if (ABS(ft_strcmp(key, g_environ[i])) == '=')
		{
			swap = ft_strsplit(g_environ[i], '=');
			free(g_environ[i]);
			g_environ[i] = ft_strings_join(2, "=", key, value);
			free_array(swap);
		}
	chfree(dummy);
	return (0);
}

char	**remove_env(char *name, int environ_len)
{
	int		i;
	int		j;
	char	**new_env;

	new_env = (char **)ft_memalloc(sizeof(char *) * environ_len);
	i = 0;
	j = 0;
	while (g_environ[i])
	{
		if (ABS(ft_strcmp(g_environ[i], name)) != '=')
			new_env[j++] = g_environ[i];
		else
			free(g_environ[i]);
		i++;
	}
	return (new_env);
}

int		unset_env(char *name)
{
	int		len;
	char	**new_env;
	char	*dummy;

	dummy = get_env(name);
	if (dummy == NULL)
		return (0);
	len = 0;
	while (g_environ[len])
		len++;
	new_env = remove_env(name, len);
	chfree_n(2, g_environ, dummy);
	g_environ = new_env;
	return (0);
}

char	**copy_env(char **argenv, char **globalenv)
{
	int		i;
	char	**new;
	char	**environ;

	environ = argenv != NULL ? argenv : globalenv;
	i = 0;
	while (environ[i++])
		;
	new = (char **)ft_memalloc(sizeof(char *) * (i--));
	i = 0;
	while (environ[i])
	{
		new[i] = ft_strdup(environ[i]);
		i++;
	}
	return (new);
}
