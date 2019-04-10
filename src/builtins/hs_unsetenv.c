/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 15:35:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int				hs_unsetenv(const char **args)
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
			unset_env_v(g_term->context_original->environ, args[i++]);
	return (0);
}
