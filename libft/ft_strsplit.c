/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/31 13:32:09 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/04/01 17:00:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_length(char const *str, char delim)
{
	size_t	i;

	i = 0;
	while (str[i] && str[i] != delim)
		i++;
	return (i);
}

static int		count_words(char const *str, char delim)
{
	int		words;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen((char *)str);
	words = 0;
	if (str[0] != delim)
		words++;
	if (len == 0)
		return (0);
	while (i < len - 1)
	{
		if (str[i] == delim && str[i + 1] != delim && str[i + 1] != '\0')
			words++;
		i++;
	}
	return (words);
}

char			**ft_strsplit(char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	word_len;
	char	**splitted;

	j = 0;
	i = 0;
	if (s == NULL)
		return (NULL);
	splitted = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (splitted == NULL)
		return (NULL);
	while (i < ft_strlen((char *)s))
	{
		if (s[i] != c)
		{
			word_len = word_length(s + i, c);
			splitted[j++] = ft_strsub(s, (unsigned int)i, word_len);
			i += word_len;
		}
		i++;
	}
	splitted[j] = NULL;
	return (splitted);
}
