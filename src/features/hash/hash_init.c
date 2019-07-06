/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 23:46:18 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/05 00:16:50 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell_hashtable.h"

int	hash_init(t_hash_tab **hash_table, size_t tab_size)
{
	if (!(*hash_table = (t_hash_tab *)malloc(sizeof(t_hash_tab))))
		return (-1);
	if (!((*hash_table)->htab =
			(t_hash_node **)malloc(sizeof(t_hash_node *) * tab_size)))
	{
		free(*hash_table);
		return (-1);
	}
	ft_bzero((*hash_table)->htab, tab_size);
	(*hash_table)->size = tab_size;
	return (0);
}
