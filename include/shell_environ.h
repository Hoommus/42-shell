/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:11:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/15 16:24:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_ENVIRON_H
# define SHELL_ENVIRON_H

# pragma clang diagnostic push
# pragma ide diagnostic ignored "readability-avoid-const-params-in-decls"

# include <stdbool.h>
# include "libft.h"

# define VAR_VECTOR_INITIAL_SIZE 64

enum							e_var_scope
{
	VAR_GLOBAL = 1,
	VAR_SHELL_LOCAL = 2,
	VAR_SCRIPT_GLOBAL = 4,
	VAR_EXPORTING = 8,
	VAR_COMMAND_LOCAL = 16,
};

/*
** TODO: add capability of storing any kind of data:
**  string
**  numeric
**  lists
**  arrays
*/

typedef struct					s_var
{
	char				*key;
	char				*value;
	u_int64_t			key_hash;
	u_int64_t			hash;
	bool				is_readonly;
	enum e_var_scope	scope;
}								t_var;

struct							s_vector_generic
{
	void			*array;
	u_int32_t		capacity;
	u_int32_t		size;
};

typedef struct s_vector_generic	t_environ_vector;

t_environ_vector	*environ_create_vector(const u_int32_t capacity);
t_environ_vector	*environ_reallocate_vector(t_environ_vector *vector);

t_var				*environ_update_entry(t_environ_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope);

t_var				*environ_push_entry(t_environ_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope);

t_var				*environ_is_entry_present(t_environ_vector *vector,
	const char *key, const char *value);

int					environ_remove_entry(t_environ_vector *vector,
	const char *key);
t_var				*environ_get_entry(t_environ_vector *vector,
										const char *key);

void				environ_from_array(t_environ_vector *vector,
	const char **environ);
char				**environ_to_array(t_environ_vector *vector,
	u_int32_t scopes);

# pragma clang diagnostic pop

#endif
