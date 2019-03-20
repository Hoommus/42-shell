/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_hash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 17:35:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/13 15:38:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** This is *not* a cryptographic-strong function, so collisions are not rare and
** you must __always__ double check strings with classic strcmp
*/

u_int64_t	hash_sdbm(const char *str)
{
	unsigned long	hash;
	int				c;

	hash = 0;
	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;
	return (hash);
}
