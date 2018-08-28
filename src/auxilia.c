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

#include "../include/twenty_one_sh.h"

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

	if (get_env("PWD") == NULL)
		set_env("PWD", "");
	if (get_env("OLDPWD") == NULL)
		set_env("OLDPWD", (var = get_env("HOME")) == NULL ? "" : var);
	if (get_env("PATH") == NULL)
		set_env("PATH", "");
}

ssize_t		ponies_teleported(void)
{
	ssize_t			ponies;
	static int		fd;

	if (fd == 0)
		fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		read(fd, &ponies, sizeof(ssize_t));
		if (ponies == 0)
			ponies += 1348;
		return (ABS(ponies));
	}
}

int		display_prompt(enum e_state state)
{
	if (state == NORMAL)
		return (display_normal_prompt());
	else if (state == DQUOTE)
		return (ft_printf("dquote>"));
	else if (state == QUOTE)
		return (ft_printf("quote>"));
	else if (state == HEREDOC)
		return (ft_printf("heredoc>"));
	else if (state == ESCAPED_NL)
		return (ft_printf(">"));
	else
		return (ft_printf("err>"));
}

void		increment_shlvl(void)
{
	char		*swap;
	int			level;

	swap = get_env("SHLVL");
	if (swap == NULL || ft_strlen(swap) == 0)
		set_env("SHLVL", "1");
	else
	{
		level = ft_atoi(swap) + 1;
		set_env("SHLVL", (swap = ft_itoa(level)));
		chfree(swap);
	}
}

