/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 19:39:51 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/28 19:08:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (*src != '\0' && i < len)
	{
		dst[i] = *src++;
		i++;
	}
	while (*src == '\0' && i < len)
	{
		dst[i] = '\0';
		i++;
	}
	return (dst);
}
