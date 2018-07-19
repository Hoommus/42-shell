/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 11:03:39 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/24 11:22:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	char	*copy;
	size_t	i;

	i = 0;
	copy = s1;
	while (*copy)
		copy++;
	while (s2[i] && i < n)
	{
		*copy = s2[i];
		copy++;
		i++;
	}
	*copy = '\0';
	return (s1);
}
