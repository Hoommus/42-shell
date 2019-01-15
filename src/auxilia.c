/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxilia.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/24 14:37:37 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

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

void	display_prompt(enum e_input_state state)
{
	if (state == STATE_NORMAL)
		display_normal_prompt();
	else if (state == STATE_DQUOTE)
		ft_printf("dquote> ");
	else if (state == STATE_QUOTE)
		ft_printf("quote> ");
	else if (state == STATE_ESCAPED_EOL)
		ft_printf("> ");
	else if (state == STATE_HEREDOC)
		ft_printf("heredoc> ");
	else if (state == STATE_EMPTY_PIPE)
		ft_printf("pipe> ");
	else
		ft_printf("err> ");
	carpos_update(POS_PROMPT);
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

