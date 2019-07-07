/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_hash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 13:10:22 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/07 13:24:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"
#include "shell_hashtable.h"

int					hs_hash(const char **args)
{
	size_t		i;
	t_hash_tab	*hash_table;
	t_hash_node	*cur_node;

	args = NULL;
	hash_table = g_hash_table;
	i = 0;
	while (i < hash_table->size)
	{
		if ((cur_node = hash_table->htab[i]) != NULL)
		{
			while (cur_node)
			{
				ft_printf("%s=%s\n", cur_node->key, cur_node->value);
				cur_node = cur_node->next;
			}
		}
		i++;
	}
	return (0);
}
