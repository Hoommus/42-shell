/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_hashtable.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 23:01:52 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 18:15:59 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_HASHTABLE_H
# define SHELL_HASHTABLE_H

# include <stddef.h>
# include <stdint.h>

typedef struct	s_hash_node
{
	char				*key;
	char				*value;
	struct s_hash_node	*next;
}				t_hash_node;

typedef struct	s_hash_tab
{
	t_hash_node	**htab;
	size_t		size;
}				t_hash_tab;

extern t_hash_tab		*g_hash_table;

int				hash_init(t_hash_tab **hash_table, size_t tab_size);
int				hash_add(t_hash_tab *hash_table,
							const char *key, const char *value);
int				hash_get(t_hash_tab *hash_table, const char *key,
							char *value_buf, size_t buf_size);
void			hash_trunc(t_hash_tab *hash_table);
void			hash_free(t_hash_tab *hash_table);

uint64_t		hash_sdbm(const char *str);

#endif
