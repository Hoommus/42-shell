/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:19:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/08 13:19:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

int		hs_export(const char **args)
{
	const t_var		*vars = (t_var *)g_term->context_current->environ->array;
	u_int32_t		i;
	bool			is_p;

	if (!(args == NULL || *args == NULL || ft_strncmp(*args, "-", 1) == 0))
		return (hs_setenv(args));
	i = 0;
	is_p = flag_short_present(args, 'p');
	while (i < g_term->context_current->environ->size)
	{
		if (vars[i].scope & SCOPE_EXPORT)
		{
			if (is_p)
				ft_printf("export ");
			print_var(vars + i);
		}
		i++;
	}
	return (0);
}
