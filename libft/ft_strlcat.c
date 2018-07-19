/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 11:27:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/29 13:36:23 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dstsize)
{
	char	*copy;
	size_t	i;
	size_t	dstlen;

	i = 0;
	dstlen = ft_strlen(dst);
	copy = dst;
	while (*copy)
		copy++;
	if (dstsize < dstlen)
		return (dstsize + ft_strlen((char *)src));
	while (src[i] && (i + 1) < dstsize - dstlen)
	{
		*copy = src[i];
		copy++;
		i++;
	}
	*copy = '\0';
	return (dstlen + ft_strlen((char *)src));
}
