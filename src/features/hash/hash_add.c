/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 23:14:43 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/05 10:28:05 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell_hashtable.h"

static int	create_node(const char *key, const char *value, t_hash_node **node)
{
	if (!(*node	= (t_hash_node *)malloc(sizeof(t_hash_node))))
		return (-1);
	if (!((*node)->key = ft_strdup(key)))
	{
		free(*node);
		return (-1);
	}
	if (!((*node)->value = ft_strdup(value)))
	{
		free(*node);
		free((*node)->key);
		return (-1);
	}
	(*node)->next = NULL;
	return (0);
}

static void	free_node(t_hash_node *node)
{
	free(node->key);
	free(node->value);
	free(node);
}

static void	push_node(t_hash_node **prev_next, t_hash_node *cur)
{
	t_hash_node *tmp;

	cur->next = (*prev_next)->next;
	tmp = *prev_next;
	*prev_next = cur;
	free_node(tmp);
}

static int	add_node(t_hash_node **hash_list, t_hash_node *new_node)
{
	t_hash_node	*cursor;

	if (!*hash_list)
	{
		*hash_list = new_node;
		return (0);
	}
	if (!ft_strcmp((*hash_list)->key, new_node->key))
	{
		push_node(hash_list, new_node);
		return (0);
	}
	cursor = *hash_list;
	while (cursor->next)
	{
		if (!ft_strcmp(cursor->next->key, new_node->key))
		{
			push_node(&cursor->next, new_node);
			return (0);
		}
		cursor = cursor->next;
	}
	cursor->next = new_node;
	return (0);
}

int			hash_add(t_hash_tab *hash_table, const char *key, const char *value)
{
	t_hash_node	*new_node;
	size_t		hash_i;

	if (create_node(key, value, &new_node))
		return (-1);
	hash_i = ((size_t)hash_sdbm(key)) % hash_table->size;
	if (add_node(&hash_table->htab[hash_i], new_node))
	{
		free_node(new_node);
		return (-1);
	}
	return (0);
}
