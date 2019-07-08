/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_hash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 20:15:42 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "acompl.h"
#include "shell_hashtable.h"

int					get_hashv(char *input_str, char *hashv[])
{
	size_t		input_len;
	size_t		hashi;
	size_t		i;
	t_hash_node	*cur_node;

	input_len = ft_strlen(input_str);
	hashi = 0;
	i = 0;
	while (i < g_hash_table->size)
	{
		cur_node = g_hash_table->htab[i];
		while (cur_node)
		{
			if (ft_strnstr(cur_node->key, input_str, input_len))
				hashv[hashi++] = ft_strdup(cur_node->key);
			cur_node = cur_node->next;
		}
		i++;
	}
	hashv[hashi] = NULL;
	return (0);
}

int					get_hashc(char *input_str)
{
	size_t		input_len;
	size_t		hashc;
	size_t		i;
	t_hash_node	*cur_node;

	input_len = ft_strlen(input_str);
	hashc = 0;
	i = 0;
	while (i < g_hash_table->size)
	{
		cur_node = g_hash_table->htab[i];
		while (cur_node)
		{
			if (ft_strnstr(cur_node->key, input_str, input_len))
				hashc++;
			cur_node = cur_node->next;
		}
		i++;
	}
	return (hashc);
}
