/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/30 13:28:54 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/30 13:39:53 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr(int n)
{
	int		curr;
	int		next;

	if (n == -2147483648)
	{
		ft_putnbr(-214748364);
		ft_putnbr(8);
		return ;
	}
	else if (n < 0)
	{
		ft_putchar('-');
		n = -1 * n;
	}
	curr = n % 10;
	next = n / 10;
	if (next != 0)
		ft_putnbr(next);
	ft_putchar(curr + '0');
}
