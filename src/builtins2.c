/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/08/01 12:52:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int				hs_env(char **args)
{
	char		*str;

	str = ft_strarr_join("\n", g_environ);
	ft_printf("%s\n", str);
	free(str);
	*args = args[0];
	return (0);
}

int				hs_setenv(char **args)
{
	int		len;
	char	**swap;

	len = 0;
	while (args[len])
		len++;
	if (len > 3 || len == 0)
		ft_printf("usage:\nsetenv [KEY=VALUE | KEY = VALUE | KEY VALUE]\n");
	else if (len == 2)
		set_env(args[0], args[1]);
	else if (len == 3)
		set_env(args[0], args[2]);
	else
	{
		swap = ft_strsplit(args[0], '=');
		if (ft_strlen(swap[1]) == 0)
			set_env(swap[0], "''");
		else if (swap[2] == NULL)
			set_env(swap[0], swap[1]);
		else
			ft_dprintf(2, "setenv: arguments not valid in this context.\n");
		free_array(swap);
	}
	return (0);
}

int				hs_unsetenv(char **args)
{
	int		len;
	int		i;

	i = 0;
	len = 0;
	while (args[len])
		len++;
	if (len == 0)
		ft_printf("usage:\nunsetenv KEY [KEY ...]\n");
	else
		while (i < len)
			unset_env(args[i++]);
	return (0);
}
