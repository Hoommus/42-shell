/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 17:17:26 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

static int32_t	count_substrings(const char *const str,
	const char *const delims)
{
	size_t	len;
	size_t	i;
	int		subs;
	char	c;

	subs = 0;
	i = 0;
	len = ft_strlen((char *)str);
	while (str && i < len)
	{
		c = str[i];
		if (ft_strchr(LIBFT_WHTSP, str[i]) != NULL && ++i)
			continue ;
		else if (ISQT(str[i]) && ++i)
			while (str[i] && str[i] != c)
				i++;
		else
			while (str[i] && ft_strchr(delims, str[i]) == NULL)
				i++;
		subs++;
		i++;
	}
	return (subs);
}

static int64_t	get_word_size(const char *const str, const char *const delims)
{
	long long	i;
	char		quote;

	quote = *str;
	i = 0;
	if (ISQT(*str) && ++i)
		while (str[i] && str[i] != quote)
			i++;
	else if (ft_isdigit(*str) && ++i)
		while (str[i] && ft_isalnum(str[i]))
			i++;
	else
		while (str[i] && !ft_strchr(delims, str[i]))
			i += (str[i] == '\\') ? 2 : 1;
	return (i);
}

/*
** Split that takes into account quotes ("", '', ``), separators - ';', '\n'
** and brackets
** TODO: Try to fix that too high memory allocation thing
** TODO: Remove this shit
*/

char			**smart_split(const char *const str, const char *const delims)
{
	char		**array;
	int64_t		j;
	int64_t		i;
	int64_t		word_size;
	int64_t		subs;

	array = (char **)ft_memalloc(sizeof(char *) *
				(subs = count_substrings(str, delims) + 2));
	j = 0;
	i = -1;
	while (str[++i] && j < subs - 1)
	{
		if (ft_strchr(delims, str[i]) != NULL)
			continue ;
		word_size = get_word_size(str + i, delims);
		array[j++] = ft_strsub(str + i, 0, word_size + ISQT(str[i]));
		if (str[i] != ';')
			i += word_size - !ISQT(str[i]);
	}
	array[j] = NULL;
	return (array);
}
