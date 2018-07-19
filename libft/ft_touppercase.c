/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_touppercase.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 15:38:40 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/19 15:38:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_touppercase(char *string)
{
	int		i;

	i = 0;
	while (string[i])
	{
		string[i] = (char)ft_toupper(string[i]);
		i++;
	}
	return (string);
}
