/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 14:40:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/28 16:18:16 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	int		beg;
	int		i;
	int		j;

	if (ft_strlen((char *)needle) == 0)
		return ((char *)haystack);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		beg = i;
		while (haystack[i] && haystack[i] == needle[j])
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
