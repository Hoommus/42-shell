/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strequ.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 15:08:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/29 15:10:46 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strequ(char const *a, char const *b)
{
	if (a == NULL || b == NULL)
		return (0);
	if (ft_strlen((char *)a) == ft_strlen((char *)b) && ft_strcmp(a, b) != 0)
		return (0);
	else
		return (1);
}
