/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:42:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/14 16:35:58 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

char	*expand_quotes(char *str)
{
	u_int32_t	i;
	u_int32_t	k;
	size_t		len;

	len = ft_strlen(str);
	str = ft_strdup(str);
	i = -1;
	while (++i < len)
		if (ft_strchr("\"'", str[i]) != NULL)
		{
			k = i + 1;
			while (str[k] != str[i] && k < len)
				k += str[k] == '\\' ? 2 : 1;
			if ((str[i] == '"' && str[k] == '"') ||
				(str[i] == '\'' && str[k] == '\''))
			{
				ft_memmove(str + i, str + i + 1, ft_strlen(str + i));
				ft_memmove(str + k - 1, str + k, ft_strlen(str + k - 1));
				i = k - 2;
			}
			len = ft_strlen(str);
		}
	return (str);
}
