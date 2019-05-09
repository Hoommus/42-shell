/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ_memory.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/04 16:03:19 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/04 16:04:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_environ.h"

/*
** TODO: Add binary search instead of classic iteration
**       For this sort entries in the table by their hash
*/

t_env_vector		*environ_create_vector(const u_int32_t capacity)
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

void				environ_deallocate_vector(t_env_vector *vector)
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

t_env_vector		*environ_reallocate_vector(t_env_vector *vector)
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
