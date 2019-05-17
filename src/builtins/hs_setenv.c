/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 15:35:53 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/11 12:40:42 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

extern bool		g_is_subshell_env;

static int		set_tricky(const char *str)
{
	t_env_vector		*vector;
	char				tmp[1024];
	int					j;

	vector = g_is_subshell_env ? g_term->context_current->environ
								: g_term->context_original->environ;
	ft_bzero(tmp, sizeof(char) * 1024);
	j = -1;
	while (str[++j])
		if (j >= 1024)
			return ((ft_dprintf(2, ANSI_RESET
			"setenv: variable name is too long\n") & 0) | 1);
		else if ((str[j] == '=' || !str[j]) &&
			is_valid_var(ft_memcpy(tmp, str, j)))
		{
			set_env_v((t_env_vector *)vector, tmp, str + j + 1, SCOPE_EXPORT);
			break ;
		}
		else if (str[j] == '=' && !is_valid_var(tmp))
		{
			ft_dprintf(2, "setenv: '%s' is not a valid variable name\n",
				ft_strlen(tmp) == 0 ? str : tmp);
			return (1);
		}
	return (0);
}

int				hs_setenv(const char **args)
{
	int					i;

	i = 0;
	if (args[0] == NULL)
		ft_printf("usage:\nsetenv [KEY=VALUE ...]\n");
	while (args[i])
		if (set_tricky(args[i++]) == 1)
			return (1);
	return (0);
}
