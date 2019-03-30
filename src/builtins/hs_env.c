/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 15:35:23 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 16:40:03 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

int			hs_env(char **args)
{
	const t_var		*vars = (t_var *)g_term->context_current->environ->array;
	u_int32_t		i;

	i = 0;
	while (i < g_term->context_current->environ->size)
	{
		if (vars[i].scope & SCOPE_EXPORT)
			print_var(vars + i);
		i++;
	}
	*args = args[0];
	return (0);
}
