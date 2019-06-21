/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_unalias.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/21 12:28:55 by obamzuro          #+#    #+#             */
/*   Updated: 2019/06/21 12:33:06 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_builtins.h"
#include "twenty_one_sh.h"

int			hs_unalias(const char **args)
{
	extern t_env_vector	*g_alias_vector;
	u_int32_t			i;

	i = -1;
	while (args[++i])
		environ_remove_entry(g_alias_vector, args[i]);
	return (0);
}
