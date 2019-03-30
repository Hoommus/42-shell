/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_environ.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 18:11:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/25 15:37:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_ENVIRON_H
# define SHELL_ENVIRON_H

# pragma clang diagnostic push
# pragma ide diagnostic ignored "readability-avoid-const-params-in-decls"

# include <stdbool.h>
# include "libft.h"

# define VARIABLES_VECTOR_INITIAL_SIZE 64

enum							e_var_scope
{
	SCOPE_GLOBAL = 1,
	SCOPE_SHELL_LOCAL = 2,
	SCOPE_SCRIPT_GLOBAL = 4,
	SCOPE_EXPORT = 8,
	SCOPE_COMMAND_LOCAL = 16,
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

typedef struct s_vector_generic	t_env_vector;

t_env_vector					*environ_create_vector(const u_int32_t capacity);
t_env_vector					*environ_reallocate_vector(t_env_vector *vector);
void							environ_deallocate_vector(t_env_vector *vector);
t_var							*environ_update_entry(t_env_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope);

t_var							*environ_push_entry(t_env_vector *vector,
	const char *key, const char *value, const enum e_var_scope scope);

t_var							*environ_is_entry_present(t_env_vector *vector,
	const char *key, const char *value);

int								environ_remove_entry(t_env_vector *vector,
	const char *key);
t_var							*environ_get_entry(t_env_vector *vector,
	const char *key);

void							environ_from_array(t_env_vector *vector,
	const char **environ);
char							**environ_to_array(t_env_vector *vector,
	u_int32_t scopes);
void							environ_import(t_env_vector *dst,
	const t_env_vector *src, const u_int32_t scopes);

# pragma clang diagnostic pop

#endif
