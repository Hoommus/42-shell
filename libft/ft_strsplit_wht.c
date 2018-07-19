/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit_wht.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/18 20:13:19 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/18 20:16:04 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_length(char const *str)
{
	size_t	i;

	i = 0;
	while (str[i] && ft_strchr(LIBFT_WHTSP, str[i]) == 0)
		i++;
	return (i);
}

static int		count_words(char const *str)
{
	int		words;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen((char *)str);
	words = 0;
	if (ft_strchr(LIBFT_WHTSP, str[0]))
		words++;
	if (len == 0)
		return (0);
	while (i < len - 1)
	{
		if (ft_strchr(LIBFT_WHTSP, str[i]) != 0
			&& ft_strchr(LIBFT_WHTSP, str[i + 1]) == 0
			&& str[i + 1] != '\0')
			words++;
		i++;
	}
	return (words);
}

char			**ft_strsplit_wht(char const *s)
{
	size_t	i;
	size_t	j;
	size_t	word_len;
	char	**splitted;

	j = 0;
	i = 0;
	if (s == NULL)
		return (NULL);
	splitted = (char **)malloc(sizeof(char *) * (count_words(s) + 1));
	if (splitted == NULL)
		return (NULL);
	while (i < ft_strlen((char *)s))
	{
		if (ft_strchr(LIBFT_WHTSP, s[i]) == 0)
		{
			word_len = word_length(s + i);
			splitted[j++] = ft_strsub(s, (unsigned int)i, word_len);
			i += word_len;
		}
		i++;
	}
	splitted[j] = NULL;
	return (splitted);
}
