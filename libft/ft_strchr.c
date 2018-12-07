/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 18:41:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/16 13:18:55 by vtarasiu         ###   ########.fr       */
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

/*
** Returns pointer to any found needle char in str.
*/

char	*ft_strchr_any(const char *str, const char *needles)
{
	unsigned long long	i;
	unsigned long long	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (needles[j])
			if (str[i] == needles[j++])
				return ((char *)str + i);
		i++;
	}
	return ((void *)0);
}
