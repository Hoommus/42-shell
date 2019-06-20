/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/17 14:36:09 by obamzuro          #+#    #+#             */
/*   Updated: 2019/06/20 20:59:41 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_builtins.h"
#include "twenty_one_sh.h"

t_env_vector *g_alias_vector;

static void	hs_alias_print_all(void)
{
	uint32_t		i;
	t_var			*var;

	i = -1;
	while (++i < g_alias_vector->size)
	{
		var = ((t_var *)g_alias_vector) + i;
		print_var(var);
	}
}

static void	hs_alias_print_one(const char *key)
{
	t_var	*var;

	var = environ_get_entry(g_alias_vector, key);
	if (var)
		print_var(var);
	else
		ft_printf("alias: %s: not found", key);
}

void		hs_alias_kernel(const char *arg)
{
	char			*equal_sign;
	char			*key;
	char			*value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		hs_alias_print_one(arg);
	else
	{
		key = ft_strsub(arg, 0, equal_sign - arg);
		value = ft_strdup(equal_sign + 1);
		if (equal_sign == arg)
			ft_printf("alias: %s: not found", value);
		else
			environ_push_entry(g_alias_vector, key, value, 0);
		free(key);
		free(value);
	}
}

int			hs_alias(const char **args)
{
	u_int32_t		i;

	if (!args[0])
		hs_alias_print_all();
	else
	{
		i = 0;
		while (args[++i])
			hs_alias_kernel(args[i]);
	}
	return (0);
}
