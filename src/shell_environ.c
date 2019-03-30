/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:39:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 14:36:47 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_environ.h"
#include "twenty_one_sh.h"

/*
** if vector is null, uses current context
*/

t_var	*get_env_v(t_env_vector *vector, const char *key)
{
	t_var		*vars;
	u_int64_t	hash;
	u_int32_t	i;

	hash = hash_sdbm(key);
	if (vector == NULL)
		vector = g_term->context_current->environ;
	if (vector == NULL)
		return (NULL);
	vars = (t_var *)vector->array;
	i = -1;
	while (++i < vector->size)
		if (vars[i].key_hash == hash && ft_strcmp(vars[i].key, key) == 0)
			return (vars + i);
	return (NULL);
}

int		set_env_v(t_env_vector *vector, const char *key, const char *value,
	enum e_var_scope scope)
{
	if (vector == NULL)
		vector = g_term->context_current->environ;
	if (vector == NULL)
		return (0);
	return ((int)environ_push_entry(vector, key, value, scope));
}

int		unset_env_v(t_env_vector *vector, const char *key)
{
	if (key == NULL || environ_get_entry(vector, key) == NULL)
		return (1);
	environ_remove_entry(vector, key);
	return (0);
}
