/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ_vector.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 17:26:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 14:30:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_environ.h"

/*
** TODO: make use of hashes
** seems like I did it, but in a very limited way
*/

t_environ_vector	*environ_create_vector(const u_int32_t capacity)
{
	t_environ_vector	*vector;

	vector = ft_memalloc(sizeof(t_environ_vector));
	vector->capacity = capacity;
	vector->size = 0;
	vector->array = (t_var *)ft_memalloc(capacity * sizeof(t_var));
	return (vector);
}

t_environ_vector	*environ_reallocate_vector(t_environ_vector *vector)
{
	t_var				*array;
	size_t				array_size;

	vector->capacity = vector->capacity + (vector->capacity >> 1);
	array = (t_var *)ft_memalloc(vector->capacity * sizeof(t_var));
	array_size = sizeof(t_var) * vector->size;
	ft_memcpy(array, vector->array, array_size);
	free(vector->array);
	vector->array = array;
	return (vector);
}

t_var				*environ_update_entry(t_environ_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope)
{
	t_var	*entry;
	char	*tmp;

	entry = environ_get_entry(vector, key);
	ft_memdel((void **)&(entry->value));
	entry->scope = scope;
	entry->value = ft_strdup(value);
	tmp = ft_strjoin(key, value);
	entry->hash = hash_sdbm(tmp) + entry->scope;
	free(tmp);
	return (entry);
}

t_var				*environ_push_entry(t_environ_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope)
{
	t_var	*entry;
	char	*tmp;

	if (vector->size >= vector->capacity - 2)
		environ_reallocate_vector(vector);
	if (environ_get_entry(vector, key))
		entry = environ_update_entry(vector, key, value, scope);
	else
	{
		entry = ft_memalloc(sizeof(t_var));
		entry->key = ft_strdup(key);
		entry->value = ft_strdup(value);
		entry->scope = scope;
		entry->key_hash = hash_sdbm(key);
		tmp = ft_strjoin(key, value);
		entry->hash = hash_sdbm(tmp) + entry->scope;
		((t_var *)vector->array)[vector->size] = *entry;
		vector->size++;
		free(entry);
		free(tmp);
	}
	return (entry);
}

t_var				*environ_get_entry(t_environ_vector *vector,
	const char *key)
{
	const t_var *const	vars = (const t_var *)vector->array;
	u_int32_t			i;
	u_int64_t			hash = hash_sdbm(key);

	i = -1;
	while (++i < vector->size)
		if (vars[i].key_hash == hash && ft_strcmp(key, vars[i].key) == 0)
			return ((t_var *)vars + i);
	return (NULL);
}

t_var				*environ_is_entry_present(t_environ_vector *vector,
	const char *key, const char *value)
{
	t_var	*entry;

	if ((entry = environ_get_entry(vector, key)) &&
		ft_strcmp(entry->value, value) == 0)
		return (entry);
	return (NULL);
}

int					environ_remove_entry(t_environ_vector *vector,
	const char *key)
{
	const t_var *const	vars = (const t_var *)vector->array;
	u_int64_t			hash;
	u_int32_t			i;

	hash = hash_sdbm(key);
	i = -1;
	while (++i < vector->size)
		if (vars[i].key_hash == hash && ft_strcmp(key, vars[i].key) == 0)
		{
			ft_memdel((void **)&(vars[i].key));
			ft_memdel((void **)&(vars[i].value));
			ft_memdel((void **)&(vars[i]));
			if (i < vector->size - 1)
				ft_memmove((t_var *)vars + i, vars + i + 1,
					sizeof(t_var) * (vector->size - i));
			return (0);
		}
	return (1);
}
