/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilia.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/08/01 13:15:07 by vtarasiu         ###   ########.fr       */
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

void		restore_variables(void)
{
	char	*var;

	if ((var = get_env("PWD")) == NULL)
		set_env("PWD", "");
	chfree(var);
	if ((var = get_env("OLDPWD")) == NULL)
		set_env("OLDPWD", (var = get_env("HOME")) == NULL ? "" : var);
	chfree(var);
	if ((var = get_env("PATH")) == NULL)
		set_env("PATH", "");
	chfree(var);
}

void		increment_shlvl(void)
{
	char		*swap;
	int			level;

	swap = get_env("SHLVL");
	if (swap == NULL)
		set_env("SHLVL", "1");
	else
	{
		level = ft_atoi(swap) + 1;
		free(swap);
		set_env("SHLVL", (swap = ft_itoa(level)));
		chfree(swap);
	}
}
