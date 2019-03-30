/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 17:04:03 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/19 14:29:25 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** size_t			ft_strlen(char *str)
** {
** 	unsigned long	i;
**
** 	i = 0;
** 	while (str && str[i])
** 		i++;
** 	return (i);
** }
*/

static size_t	consider_return(const char *original, unsigned long longword,
									unsigned long *longswap)
{
	static unsigned long	high = ((0x80808080L << 16) << 16) | 0x80808080L;
	static unsigned long	low = ((0x01010101L << 16) << 16) | 0x01010101L;
	char					*char_ptr;

	if (((longword - low) & ~longword & high) != 0)
	{
		char_ptr = (char *)(longswap - 1);
		if (char_ptr[0] == 0)
			return (char_ptr - original);
		if (char_ptr[1] == 0)
			return (char_ptr - original + 1);
		if (char_ptr[2] == 0)
			return (char_ptr - original + 2);
		if (char_ptr[3] == 0)
			return (char_ptr - original + 3);
		if (char_ptr[4] == 0)
			return (char_ptr - original + 4);
		if (char_ptr[5] == 0)
			return (char_ptr - original + 5);
		if (char_ptr[6] == 0)
			return (char_ptr - original + 6);
		if (char_ptr[7] == 0)
			return (char_ptr - original + 7);
	}
	return (-1);
}

size_t			ft_strlen_unsafe(const char *str)
{
	const char			*swap;
	unsigned long		*longswap;
	unsigned long		longword;
	size_t				size;

	swap = str;
	while (((unsigned long)swap & (sizeof(longword) - 1)) != 0)
	{
		if (*swap == '\0')
			return (swap - str);
		swap++;
	}
	longswap = (unsigned long *)swap;
	while (1)
	{
		longword = *longswap++;
		if ((size = consider_return(str, longword, longswap))
			!= (u_int64_t) - 1)
			return (size);
	}
}

size_t			ft_strlen(const char *str)
{
	return (str == NULL ? 0 : ft_strlen_unsafe(str));
}
