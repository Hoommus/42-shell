/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 15:35:53 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 17:54:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** static int		set_tricky(const char *str)
** {
**     size_t	len;
**     int		i;
**     char		*first;
**     char		*second;
**
**     second = ft_strchr(str, '=');
**     if (second != NULL && (first = ft_strsub(str, 0, second - str))
**         && ft_strlen(first) == 0 && is_valid_var(first))
**         ;
**     else
**     	ft_dprintf(2, "setenv: arguments are not valid in this context.\n");
**     free(first);
**     return (0);
** }
*/

int				hs_setenv(const char **args)
{
	const t_env_vector	*vector = g_term->context_original->environ;
	char				**swap;
	int					l;

	l = 0;
	while (args[l])
		l++;
	if (l > 3 || l == 0)
		ft_printf("usage:\nsetenv [KEY=VALUE | KEY = VALUE | KEY VALUE]\n");
	else if (ft_strchr(args[0], '=') == 0 && !is_valid_var(args[0]))
		ft_dprintf(2, "setenv: '%s' is not a valid variable name\n", args[0]);
	else if (l == 2 || l == 3)
		set_env_v((t_env_vector *)vector, args[0], args[l - 1], SCOPE_EXPORT);
	else
	{
		swap = ft_strsplit(args[0], '=');
		if (ft_strlen(swap[1]) == 0 && is_valid_var(swap[0]))
			set_env_v((t_env_vector *)vector, swap[0], "", SCOPE_EXPORT);
		else if (swap[2] == NULL && is_valid_var(swap[0]))
			set_env_v((t_env_vector *)vector, swap[0], swap[1], SCOPE_EXPORT);
		else
			ft_dprintf(2, "setenv: arguments are not valid in this context.\n");
		free_array((void **)swap);
	}
	return (0);
}
