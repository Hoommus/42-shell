/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 15:08:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int				hs_env(char **args)
{
	const t_var *const	vars = (t_var *)g_term->context_current->environ->array;
	u_int32_t			i;

	i = 0;
	while (i < g_term->context_current->environ->size)
	{
		ft_printf("%s = %s\n", vars[i].key, vars[i].value);
		i++;
	}
	*args = args[0];
	return (0);
}

int				hs_setenv(char **args)
{
	int		len;
	char	**swap;

	len = 0;
	while (args[len])
		len++;
	if (len > 3 || len == 0)
		ft_printf("usage:\nsetenv [KEY=VALUE | KEY = VALUE | KEY VALUE]\n");
	else if (len == 2)
		set_env_v(NULL, args[0], args[1], VAR_EXPORTING);
	else if (len == 3)
		set_env_v(NULL, args[0], args[2], VAR_EXPORTING);
	else
	{
		swap = ft_strsplit(args[0], '=');
		if (ft_strlen(swap[1]) == 0)
			set_env_v(NULL, swap[0], "''", VAR_EXPORTING);
		else if (swap[2] == NULL)
			set_env_v(NULL, swap[0], swap[1], VAR_EXPORTING);
		else
			ft_dprintf(2, "setenv: arguments not valid in this context.\n");
		free_array((void **)swap);
	}
	return (0);
}

int				hs_unsetenv(char **args)
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
			unset_env_v(NULL, args[i++]);
	return (0);
}
