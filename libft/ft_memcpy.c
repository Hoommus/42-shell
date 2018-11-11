/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 16:21:37 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/10/18 13:53:38 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *restrict dst, const void *restrict src, size_t size)
{
	u_int64_t	*d;
	u_int64_t	*s;
	void		*backup;
	size_t		i;

	backup = dst;
	i = -1;
	while ((size - (++i)) % sizeof(u_int64_t) != 0)
		((u_int8_t *)dst)[i] = ((u_int8_t *)src)[i];
	d = (u_int64_t *)(dst + i);
	s = (u_int64_t *)(src + i);
	i = 0;
	while (i < size / sizeof(u_int64_t))
	{
		d[i] = s[i];
		i++;
	}
	return (backup);
}
