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

size_t	ft_utf_strlen(char *str)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = 0;
	while (str[i])
	{
		while (str[i] & 0x80)
			i++;
		if (str[i] == 0)
			break ;
		size++;
		i++;
	}
	return (size);
}
