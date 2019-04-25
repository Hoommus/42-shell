/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ_vector.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 17:26:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/17 15:35:34 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_environ.h"

/*
** TODO: Add binary search instead of classic iteration
**       For this sort entries in the table by their hash
*/

t_env_vector	*environ_create_vector(const u_int32_t capacity)
{
	t_env_vector	*vector;
	char			*swap;

	vector = ft_memalloc(sizeof(t_env_vector));
	vector->capacity = capacity;
	vector->size = 0;
	vector->array = (t_var *)ft_memalloc(capacity * sizeof(t_var));
	ft_bzero(vector->array, sizeof(t_var) * vector->capacity);
	environ_push_entry(vector, "env_vector_capacity", swap = ft_itoa(capacity),
						 SCOPE_SHELL_LOCAL);
	free(swap);
	return (vector);
}

void			environ_deallocate_vector(t_env_vector *vector)
{
	t_var		*vars;
	u_int32_t	i;

	vars = vector->array;
	i = 0;
	while (i < vector->size)
	{
		ft_memdel((void **)&(vars[i].key));
		ft_memdel((void **)&(vars[i].value));
		i++;
	}
	ft_memdel((void **)&(vector->array));
	ft_memdel((void **)&(vector));
}

t_env_vector	*environ_reallocate_vector(t_env_vector *vector)
{
	t_var			*array;
	size_t			array_size;
	char			*swap;

	vector->capacity = vector->capacity + (vector->capacity >> 1);
	array = (t_var *)ft_memalloc(vector->capacity * sizeof(t_var));
	array_size = sizeof(t_var) * vector->size;
	ft_memcpy(array, vector->array, array_size);
	free(vector->array);
	vector->array = array;
	environ_update_entry(vector, "env_vector_capacity",
		swap = ft_itoa(vector->capacity),
		SCOPE_SHELL_LOCAL);
	free(swap);
	return (vector);
}

/*
** The difference between this function and set_env_v is that set_env_v
** checks if name is good.
*/

t_var				*environ_update_entry(t_env_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope)
{
	t_var *entry;
	char *tmp;

	entry = environ_get_entry(vector, key);
	ft_memdel((void **)&(entry->value));
	entry->scope = scope;
	entry->value = ft_strdup(value);
	tmp = ft_strjoin(key, value);
	entry->hash = hash_sdbm(tmp) + entry->scope;
	free(tmp);
	return (entry);
}

// TODO: FIX: Sort loses some entries
//void				insert_with_sort(t_env_vector *vector, t_var *var)
//{
//	t_var			*array;
//	u_int32_t		i;
//
//	array = vector->array;
//	i = 0;
//	while (i < vector->size && array[i].key)
//	{
//		if (ft_strcmp(array[i].key, var->key) > 0)
//		{
//			ft_memmove(array + i + 1, array + i,
//				sizeof(t_var) * (vector->size - i));
//			break ;
//		}
//		i++;
//	}
//	array[i] = *var;
//}

t_var				*environ_push_entry(t_env_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope)
{
	t_var	*entry;
	char	*tmp;

	entry = NULL;
	if (vector->size >= vector->capacity - 2)
		environ_reallocate_vector(vector);
	if (environ_get_entry(vector, key))
		entry = environ_update_entry(vector, key, value, scope);
	else if (is_valid_var((char *)key) || scope == SCOPE_SHELL_LOCAL)
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

t_var				*environ_get_entry(t_env_vector *vector,
	const char *key)
{
	const t_var *const	vars = (const t_var *)vector->array;
	u_int32_t			i;
	u_int64_t			hash;

	hash = hash_sdbm(key);
	i = -1;
	while (++i < vector->size)
		if (vars[i].key_hash == hash && ft_strcmp(key, vars[i].key) == 0)
			return ((t_var *)vars + i);
	return (NULL);
}

t_var				*environ_is_entry_present(t_env_vector *vector,
	const char *key, const char *value)
{
	t_var	*entry;

	if ((entry = environ_get_entry(vector, key)) &&
		ft_strcmp(entry->value, value) == 0)
		return (entry);
	return (NULL);
}

int					environ_remove_entry(t_env_vector *vector,
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
			vector->size--;
			return (0);
		}
	return (1);
}
