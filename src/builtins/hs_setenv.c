/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 15:35:53 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/03 18:40:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int		set_tricky(const char *str)
{
	const t_env_vector	*vector = g_term->context_original->environ;
	const char			tmp[1025] = {0};
	int					j;

	j = -1;
	while (str[++j])
		if (str[j] == '=' && is_valid_var(ft_memcpy((void *)tmp, str, j)))
			set_env_v((t_env_vector *)vector, tmp, str + j + 1, SCOPE_EXPORT);
		else if (!is_valid_var(tmp))
		{
			ft_dprintf(2, "setenv: `%s' is not a valid variable name\n", tmp);
			return (1);
		}
	return (0);
}

int				hs_setenv(const char **args)
{
	int					i;

	i = 0;
	if (args[1] == NULL)
		ft_printf("usage:\nsetenv [KEY=VALUE ...]\n");
	while (args[++i])
		if (set_tricky(args[i]) == 1)
			return (1);
	return (0);
}
