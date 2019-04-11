/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_hash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 17:35:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/10 15:45:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** This is *not* a cryptographic-strong function, so collisions are not rare and
** you must __always__ double check strings with classic strcmp
*/

u_int64_t	hash_sdbm(const char *str)
{
	u_int64_t		hash;
	u_int32_t		c;

	hash = 0;
	while ((c = (u_int32_t)(*str++)))
		hash = c + (hash << 6) + (hash << 16) - hash;
	return (hash);
}
