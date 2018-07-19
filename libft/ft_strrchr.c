/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 12:21:43 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/24 12:40:16 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	to_find;
	int		i;
	char	*last;

	last = 0;
	i = 0;
	to_find = (char)c;
	while (s[i])
	{
		if (s[i] == to_find)
			last = (char *)(s + i);
		i++;
	}
	if (to_find != 0 && last != 0)
		return (last);
	else if (to_find == 0)
		return ((char *)(s + i));
	else
		return (NULL);
}
