/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 23:44:35 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/05 00:03:33 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell_hashtable.h"

void	hash_free(t_hash_tab *hash_table)
{
	hash_trunc(hash_table);
	free(hash_table->htab);
	free(hash_table);
}
