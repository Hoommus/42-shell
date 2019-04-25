/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 16:42:04 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/22 18:49:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansions_internal.h"
#include "twenty_one_sh.h"

char	*expand_quote(char *string)
{
	u_int32_t	i;
	const char	q = '\'';

	if (ft_strchr(string, q) == NULL)
		return (ft_strdup(string));
	string = ft_strdup(string);
	i = 0;
	while (string[i] && string[i] != q)
		i++;
	ft_memmove(string + i, string + i + 1, ft_strlen(string + i) + 1);
	while (string[i] && string[i] != q)
		i++;
	ft_memmove(string + i, string + i + 1, ft_strlen(string + i) + 1);
	return (string);
}

char	*expand_dquote(char *string)
{
	char		*swap;
	char		*tmp;
	u_int32_t	i;
	u_int32_t	k;
	const char	q = '\"';

	if (ft_strchr(string, q) == NULL)
		return (ft_strdup(string));
	string = ft_strdup(string);
	i = 0;
	while (string[i] && string[i] != q)
		i += string[i] == '\\' ? 2 : 1;
	k = i + 1;
	while (string[k] && string[k] != q)
		k += string[k] == '\\' ? 2 : 1;
	if (string[k] == '\0')
		return (ft_strdup(string));
	swap = ft_strsub(string + i + 1, 0, k - i - 1);
	tmp = expand_vars(swap);
	ft_memdel((void **)&swap);
	string[i] = 0;
	swap = ft_strings_join(3, "", string, tmp, string + k + 1);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&string);
	return (swap);
}
