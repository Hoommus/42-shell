/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 18:13:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/04/01 16:33:05 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strdup(const char *src)
{
	size_t	l;
	char	*copy;
	size_t	i;

	i = 0;
	l = ft_strlen((char *)src);
	copy = (char *)malloc(sizeof(char) * (l + 1));
	if (copy == 0)
		return (0);
	while (src[i])
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
