/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolowercase.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 15:37:07 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/19 15:38:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_tolowercase(char *string)
{
	int		i;

	i = 0;
	while (string[i])
	{
		string[i] = (char)ft_tolower(string[i]);
		i++;
	}
	return (string);
}
