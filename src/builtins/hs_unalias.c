/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_unalias.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 21:57:42 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/04 12:58:48 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "shell_builtins.h"
#include "twenty_one_sh.h"

int				hs_unalias(const char **args)
{
	u_int32_t	i;
	int			status;

	status = 0;
	if (args == NULL || args[0] == NULL)
	{
		ft_dprintf(2, "unalias: usage:\nunalias [-a] name [name ...]\n");
		return (1);
	}
	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "-a") == 0)
		{
			alias_remove_all();
			break ;
		}
		if (alias_remove(args[i]))
			status = 1;
		i++;
	}
	return (status);
}
