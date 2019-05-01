/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strinsert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:43:59 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/30 12:18:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strinsert(const char *dst, const char *insertion,
			size_t region_start, size_t region_size)
{
	char	*final;

	if (!dst || !insertion)
		return ((char *)dst);
	final = ft_strnew(ft_strlen(dst) + ft_strlen(insertion) - region_size + 2);
	ft_strncat(final, dst, region_start);
	ft_strcat(final + region_start, insertion);
	ft_strncat(final + region_start + ft_strlen(insertion),
				dst + region_start + region_size,
				ft_strlen(dst + region_start + region_size));
	return (final);
}
