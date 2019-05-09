/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 14:40:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/07 17:09:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	needle_len;
	size_t	haystack_len;

	if ((needle_len = ft_strlen((char *)needle)) == 0)
		return ((char *)haystack);
	haystack_len = ft_strlen(haystack);
	i = -1;
	while (++i < haystack_len)
		if (ft_strncmp(haystack + i, needle, needle_len) == 0)
			return ((char *)(haystack + i));
	return (NULL);
}
