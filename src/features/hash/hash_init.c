/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 23:46:18 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/07 14:59:22 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell_hashtable.h"

int	hash_init(t_hash_tab **hash_table, size_t tab_size)
{
	if (!(*hash_table = (t_hash_tab *)ft_memalloc(sizeof(t_hash_tab))))
		return (-1);
	if (!((*hash_table)->htab =
			(t_hash_node **)ft_memalloc(sizeof(t_hash_node *) * tab_size)))
	{
		free(*hash_table);
		return (-1);
	}
	ft_bzero((*hash_table)->htab, tab_size * sizeof(t_hash_node));
	(*hash_table)->size = tab_size;
	return (0);
}
