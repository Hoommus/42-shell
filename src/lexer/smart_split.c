/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:44:48 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/05 13:34:35 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

#include <assert.h>

char		*g_singles[] = {
	"|",
	";",
	"\n",
	"(",
	")",
	"[",
	"]",
	"{",
	"}",
	NULL
};

/*
** Returns true if char needs separate token
*/

static int				is_single_token(char c)
{
	int		i;
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = 0;
	i = 0;
	while (g_singles[i])
		if (ft_strcmp(g_singles[i++], tmp) == 0)
			return (1);
	return (0);
}

/*
** There is some cringy stuff going on today
*/

static int				count_substrings(const char *str)
{
	size_t	i;
	int		subs;
	char	c;

	subs = 0;
	i = 0;
	size_t len = ft_strlen((char *)str);
	while (str && i < len)
	{
		c = str[i];
		if (ft_strchr(LIBFT_WHTSP, str[i]) != NULL && ++i)
			continue ;
		else if (is_single_token(str[i++]))
			subs++;
		else if (ISQT(str[i]) && ++i)
			while (str[i] && str[i] != c)
				i++;
		else
			while (str[i] && ft_strchr(TOKEN_DELIMITERS, str[i]) == NULL
				&& !is_single_token(str[i]))
				i++;
		subs++;
		i++;
	}
	return (subs);
}

static long long		get_word_size(const char *str)
{
	long long	i;
	char		quote;

	quote = *str;
	i = 0;
	if (is_single_token(*str))
		return (1);
	else if (ISQT(*str) && ++i)
		while (str[i] && str[i] != quote)
			i++;
	else if (ft_isdigit(*str) && ++i)
		while (str[i] && ft_isalnum(str[i]))
			i++;
	else
		while (str[i] && !is_single_token(str[i]) && !ft_strchr(TOKEN_DELIMITERS, str[i]))
			i++;
	// TODO: Don't forget about this guy
	assert(i > 0);
	return (i);
}

/*
** Split that takes into account quotes ("", '', ``), separators - ';', '\n'
** and brackets
** TODO: Try to fix that too high memory allocation thing
 * TODO: Remove this shit
*/
char					**smart_split(const char *str, const char *delimiters)
{
	char		**array;
	long long	j;
	long long	i;
	long long	word_size;
	long long	subs;

	array = (char **)ft_memalloc(sizeof(char *) *
				(subs = count_substrings(str) + 2));
	j = 0;
	i = -1;
	while (str[++i] && j < subs - 1)
	{
		if (ft_strchr(delimiters, str[i]) != NULL)
			continue ;
		word_size = get_word_size(str + i);
		array[j++] = ft_strsub(str + i, 0, word_size + ISQT(str[i]));
		if (str[i] != ';')
			i += word_size - !ISQT(str[i]);
	}
	array[j] = NULL;
	return (array);
}
