/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unicode.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 16:17:56 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

char	*to_unicode(wchar_t codepoint)
{
	char	*bytes;

	bytes = ft_strnew(4);
	bytes[0] = (char)(codepoint == 0 ? 1 : codepoint);
	if (codepoint != 0 && codepoint <= 0x7F)
		bytes[0] = (char)codepoint;
	else if (codepoint != 0 && codepoint <= 0x7FF)
	{
		bytes[0] = (char)(0xC0 | ((codepoint >> 6) & 0x1F));
		bytes[1] = (char)(0x80 | (codepoint & 0x3F));
	}
	else if (codepoint != 0 && codepoint <= 0xFFFF)
	{
		bytes[0] = (char)(0xE0 | ((codepoint >> 12) & 0x0F));
		bytes[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
		bytes[2] = (char)(0x80 | (codepoint & 0x3F));
	}
	else if (codepoint != 0 && codepoint <= 0x10FFFF)
	{
		bytes[0] = (char)(0xF0 | ((codepoint >> 18) & 0x07));
		bytes[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
		bytes[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
		bytes[3] = (char)(0x80 | (codepoint & 0x3F));
	}
	return (bytes);
}

char	*to_unicode_string(wchar_t *s)
{
	char	*str;
	char	*temp;
	int		i;

	if (s == 0)
		return (ft_strdup("(null)"));
	else if (*s == 0)
		return (ft_strdup("\0"));
	i = 0;
	str = ft_strnew(ft_wchar_strlen(s));
	while (s[i])
	{
		temp = to_unicode(s[i]);
		ft_strcat(str, temp);
		ft_strdel(&temp);
		i++;
	}
	return (str);
}
