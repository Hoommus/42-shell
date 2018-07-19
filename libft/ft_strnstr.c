/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 15:09:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/28 15:16:18 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	beg;
	size_t	i;
	size_t	j;

	if (ft_strlen((char *)needle) == 0)
		return ((char *)haystack);
	i = 0;
	while (i < len && haystack[i])
	{
		j = 0;
		beg = i;
		while (i < len && haystack[i] && haystack[i] == needle[j])
		{
			i++;
			if (needle[++j] == '\0')
				return ((char *)(haystack + beg));
		}
		i = beg;
		i++;
	}
	return (NULL);
}
