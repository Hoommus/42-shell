/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:24:26 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/28 18:36:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strcmp(const char *s1, const char *s2)
{
	while (1)
	{
		if (*s1 != 0 && (unsigned char)*s1 != (unsigned char)*s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		else if (*s1 == 0)
			return (0 - (unsigned char)*s2);
		else if (*s2 == 0)
			return ((unsigned char)*s1);
		s1++;
		s2++;
	}
	return (0);
}
