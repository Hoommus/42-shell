#include "../../include/script_lang.h"

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

int			is_single_token(char c)
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

int			count_substrings(char *str)
{
	ssize_t	i;
	int		subs;
	char	c;

	subs = 0;
	i = -1;
	while (str && str[++i])
	{
		c = str[i];
		if (ft_strchr(LIBFT_WHTSP, str[i]) != NULL && ++i)
			continue ;
		else if (is_single_token(str[i++]))
			subs++;
		else if (ISQT(str[i]) && ++i)
			while (str[i] != 0 && str[i] != c)
				i++;
		else
			while (str[i] != 0 && ft_strchr(TOKEN_DELIMITERS, str[i]) == NULL)
				i++;
		subs++;
	}
	return (subs);
}

char		*ft_strchr_any(const char *str, const char *needles)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i])
	{
		j = 0;
		while (needles[j])
			if (str[i] == needles[j++])
				return ((char *)str + i);
		i++;
	}
	return (NULL);
}

long long	get_word_size(char *str)
{
	long long	i;
	char		c;

	c = *str;
	i = 0;
	if (is_single_token(*str))
		return (1);
	else if (ISQT(*str) && ++i)
		while (str[i] && str[i] != c)
			i++;
	else
		i = (long long)(ft_strchr_any(str, TOKEN_DELIMITERS) - str);
	if (i < 0)
		return ((long long)ft_strlen(str));
	return (i);
}

/*
** Split that takes into account quotes ("", '', ``), separators - ';', '\n'
** and literally everything else like brackets
** TODO: Try to fix that too high memory allocation thing
 * TODO: Try to fix not enough memory allocation
*/
char		**smart_split(char *str, char *delimiters)
{
	char		**array;
	long long	j;
	long long	i;
	long long	word_size;
	long long	subs;

	array = (char **)ft_memalloc(sizeof(char *) * (subs = count_substrings(str) + 2));
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
