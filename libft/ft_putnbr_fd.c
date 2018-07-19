/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/30 13:37:52 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/30 13:45:16 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	int		curr;
	int		next;

	if (n == -2147483648)
	{
		ft_putnbr_fd(-214748364, fd);
		ft_putnbr_fd(8, fd);
		return ;
	}
	else if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = -1 * n;
	}
	curr = n % 10;
	next = n / 10;
	if (next != 0)
		ft_putnbr_fd(next, fd);
	ft_putchar_fd(curr + '0', fd);
}
