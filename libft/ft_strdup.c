/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 18:13:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/01 14:56:24 by vtarasiu         ###   ########.fr       */
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
	copy = (char *)ft_memalloc(sizeof(char) * (l + 1));
	if (copy == 0)
		return (0);
	ft_memcpy(copy, src, l);
	return (copy);
}
