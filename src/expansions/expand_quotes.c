/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:42:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/14 20:32:37 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

char	*expand_dquote_internals(char *str, u_int32_t from, u_int32_t to)
{
	char	*partial;
	char	*swap;

	partial = ft_strsub(str, from + 1, to - 1);
	swap = expand_escaped(partial);
	free(partial);
	partial = strinsert(str, swap, from, (to) - from + 1);
	free(str);
	free(swap);
	return (partial);
}

char	*expand_quotes(char *str)
{
	register u_int32_t	i;
	register u_int32_t	k;
	size_t				len;

	len = ft_strlen(str);
	str = ft_strdup(str);
	i = -1;
	while (++i < len)
		if (ft_strchr("\"'", str[i]) != NULL)
		{
			k = i + 1;
			while (str[k] != str[i] && k < len)
				k += str[k] == '\\' ? 2 : 1;
			if ((str[i] == '"' && str[k] == '"'))
				str = expand_dquote_internals(str, i, k);
			else if (str[i] == '\'' && str[k] == '\'')
			{
				ft_memmove(str + i, str + i + 1, ft_strlen(str + i));
				ft_memmove(str + k - 1, str + k, ft_strlen(str + k - 1));
				i = k - 2;
			}
			len = ft_strlen(str);
		}
	return (str);
}
