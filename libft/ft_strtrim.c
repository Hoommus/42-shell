/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 16:57:10 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/29 18:50:15 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_whsp(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

char		*ft_strtrim(char const *s)
{
	size_t	len;

	if (s == NULL)
		return (NULL);
	while (is_whsp(*s))
		s++;
	len = ft_strlen((char *)s);
	while (len > 0 && is_whsp(s[len - 1]))
		len--;
	return (ft_strsub(s, 0, len));
}
