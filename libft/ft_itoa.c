/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/30 13:56:05 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/31 13:29:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	nbr_len(int nbr)
{
	int		i;

	i = 1;
	while ((nbr = nbr / 10) != 0)
		i++;
	return (i);
}

static char	*handle_exception(int n)
{
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	else if (n == 0)
		return (ft_strdup("0"));
	else
		return (ft_strdup("hullo?"));
}

char		*ft_itoa(int n)
{
	char		*str;
	int			i;
	int			len;

	if (n == 0 || n == -2147483648)
		return (handle_exception(n));
	len = n < 0 ? 2 : 1;
	len += nbr_len(n);
	str = (char *)malloc(sizeof(char) * (len));
	if (str == NULL)
		return (NULL);
	str[0] = '-';
	if (n < 0)
		n = -n;
	i = len - 2;
	while (i >= 0 && n != 0)
	{
		str[i--] = '0' + n % 10;
		n /= 10;
	}
	str[len - 1] = '\0';
	return (str);
}
