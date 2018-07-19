/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 18:41:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/24 12:27:28 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	char	to_find;
	int		i;

	i = 0;
	to_find = (char)c;
	while (s[i])
		if (s[i] == to_find)
			return ((char *)(s + i));
		else
			i++;
	if (to_find == 0)
		return ((char *)(s + i));
	else
		return (0);
}
