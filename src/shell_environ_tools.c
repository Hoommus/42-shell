/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ_tools.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:37:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/16 13:15:32 by vtarasiu         ###   ########.fr       */
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

char		**environ_to_array(t_env_vector *vector, u_int32_t scopes)
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

static char	**append_rogue_env(t_env_vector *valt, char **env, size_t offset)
{
	const t_var		*vars = valt->array;
	size_t			i;

	i = -1;
	while (++i < valt->size)
		if (vars[i].key != NULL)
			env[offset++] = ft_strings_join(2, "=", vars[i].key, vars[i].value);
	return (env);
}

/*
** checks if vector from argument has any alterations of original environ
** and populates final array taking into account these alterations
*/

char		**environ_to_array_diff(t_env_vector *valt, const t_env_vector *vshell, u_int32_t scopes)
{
	t_var				*valt_var;
	char				**env;
	size_t				i;
	size_t				j;

	env = (char **)ft_memalloc(sizeof(char *) * (vshell->size + valt->size));
	i = -1;
	j = 0;
	while (++i < vshell->size)
	{
		if (!(scopes & ((t_var *)vshell->array + i)->scope))
			continue ;
		valt_var = environ_get_entry(valt, ((t_var *)vshell->array + i)->key);
		if (valt_var && (valt_var->key))
		{
			env[j++] = ft_strings_join(2, "=", valt_var->key, valt_var->value);
			environ_remove_entry(valt, valt_var->key);
		}
		else
			env[j++] = ft_strings_join(2, "=",
				((t_var *)vshell->array + i)->key,
				((t_var *)vshell->array + i)->value);
	}
	return (append_rogue_env(valt, env, j));
}

void		environ_from_array(t_env_vector *vector, const char **environ)
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
