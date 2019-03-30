/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ_tools.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:37:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 16:43:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_environ.h"

/*
** scopes argument is a bitwise 'or' separated list of e_var_scope entries
** e_var_scope entries integer representation guarantees usage of only one bit
** per scope type
*/

#include "assert.h"
#include "twenty_one_sh.h"
#include "ft_printf.h"

char	**environ_to_array(t_env_vector *vector, u_int32_t scopes)
{
	const t_var *const	vars = (const t_var *)vector->array;
	char				**array;
	u_int32_t			i;
	u_int32_t			j;
	u_int32_t			size;

	i = -1;
	size = 0;
	while (++i < vector->size)
		if (scopes & (vars[i].scope))
			size++;
	array = ft_memalloc(sizeof(char *) * (size + 1));
	i = -1;
	j = 0;
	while (++i < vector->size)
	{
		assert(j < size + 1);
		if (scopes & (vars[i].scope))
			array[j++] = ft_strings_join(2, "=", vars[i].key, vars[i].value);
	}
	return (array);
}

void	environ_from_array(t_env_vector *vector, const char **environ)
{
	char	**splitted;
	int		i;

	i = 0;
	while (environ && environ[i])
	{
		splitted = ft_strsplit(environ[i], '=');
		environ_push_entry(vector, splitted[0],
			splitted[1] ? splitted[1] : "", SCOPE_EXPORT);
		free_array((void **)splitted);
		i++;
	}
}
