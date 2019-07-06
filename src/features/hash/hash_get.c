/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 00:07:49 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/05 00:37:11 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell_hashtable.h"

int	hash_get(t_hash_tab *hash_table, const char *key,
				char *value_buf, size_t buf_size)
{
	t_hash_node	*htnode;

	htnode = hash_table->htab[((size_t)hash_sdbm(key)) % hash_table->size];
	while (htnode)
	{
		if (!ft_strcmp(htnode->key, key))
		{
			ft_strncpy(value_buf, htnode->value, buf_size - 1);
			value_buf[buf_size - 1] = '\0';
			return (0);
		}
		htnode = htnode->next;
	}
	return (1);
}
