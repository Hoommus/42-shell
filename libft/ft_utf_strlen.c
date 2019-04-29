/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utf_strlen.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/25 20:07:41 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/05/25 20:42:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Function counts actual number of characters in Unicode string without
** counting those starting with 0x80 (binary 10xxxxxx)
*/

size_t	ft_utf_strlen(const char *str)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = 0;
	while (str[i])
	{
		while ((u_int8_t)str[i] > 0x7F && (str[i] & 0x80) == (str[i] & 0xC0))
			i++;
		if (str[i] == 0)
			break ;
		size++;
		i++;
	}
	return (size);
}
