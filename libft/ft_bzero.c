/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/28 16:11:50 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/28 16:27:11 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	u_int64_t	*wide;
	size_t		i;

	i = 0;
	while ((n - i) % sizeof(u_int64_t) != 0)
		((u_int8_t *)s)[i++] = 0;
	wide = (u_int64_t *)(s + i);
	i = 0;
	while (i < n / sizeof(u_int64_t))
		wide[i++] = 0;
}
