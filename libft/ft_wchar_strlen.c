/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wchar_strlen.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/03 15:46:38 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/05/23 18:10:39 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

/*
** Counts how much space needed to store wchar_t string converted to one-byte
** char string
*/

size_t	ft_wchar_strlen(wchar_t *str)
{
	size_t	i;
	size_t	size;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] <= 0x7F)
			size++;
		else if (str[i] <= 0x7FF)
			size += 2;
		else if (str[i] <= 0xFFFF)
			size += 3;
		else if (str[i] <= 0x10FFFF)
			size += 4;
		i++;
	}
	return (size);
}
