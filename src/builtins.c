/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:45:42 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int				hs_echo(char **args)
{
	char	*str;
	char	*swap;

	if (args == NULL || args[0] == NULL)
	{
		ft_printf("\n");
		return (0);
	}
	str = ft_strarr_join(" ", args);
	swap = replace_variables(str);
	chfree(str);
	if (swap == NULL)
		return (1);
	ft_printf("%s\n", swap);
	chfree(swap);
	return (0);
}

int				hs_help(char **args)
{
	int		i;

	*args = args[0];
	ft_printf("Existing builtins:\n");
	i = 0;
	while (i < 8)
		ft_printf("%s, ", g_builtins[i++]);
	ft_printf("%s\n", g_builtins[8]);
	return (0);
}

int				hs_exit(char **args)
{
	*args = args[0];
	exit(0);
}
