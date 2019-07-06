/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_trunc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 23:23:30 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/05 00:04:55 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_hashtable.h"

#include <stdlib.h>

static void	free_hash_list(t_hash_node *hash_node)
{
	t_hash_node	*tmp;

	while (hash_node)
	{
		tmp = hash_node;
		hash_node = hash_node->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void		hash_trunc(t_hash_tab *hash_table)
{
	size_t	i;

	i = 0;
	while (i < hash_table->size)
	{
		free_hash_list(hash_table->htab[i]);
		hash_table->htab[i] = NULL;
		i++;
	}
}
