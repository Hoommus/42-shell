/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 13:07:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/06 17:35:00 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include "libft.h"

int		ft_atoi(const char *str)
{
	unsigned long	res;
	int				sign;

	sign = 1;
	res = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		res = res * 10 + *str++ - 48;
	return ((int)res * sign);
}

int64_t	ft_atoi_base(const char *str, int base)
{
	const char	*alphabet = "0123456789ABCDEF";
	int64_t		res;
	int			sign;

	sign = 1;
	res = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	if ((base == 8 || base == 16) && *str == '0')
		str++;
	if (base == 16 && (*str == 'x' || *str == 'X'))
		str++;
	while (ABS(ft_strchr(alphabet, ft_toupper(*str)) - alphabet) < base)
	{
		res = res * base +
			(int64_t)ABS(ft_strchr(alphabet, ft_toupper(*str)) - alphabet);
		str++;
	}
	return (res * sign);
}
