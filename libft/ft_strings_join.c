/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strings_join.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/19 12:14:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/19 15:47:39 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	malloc_str(size_t args_size, va_list *args,
							size_t delimiter_size, char **whereto)
{
	char		*str;
	size_t		whole_size;
	size_t		i;

	i = 0;
	whole_size = 0;
	while (i < args_size)
	{
		str = va_arg(*args, char *);
		whole_size += ft_strlen(str);
		i++;
	}
	whole_size += delimiter_size * (i - 1);
	*whereto = ft_strnew(whole_size + 1);
	va_end(*args);
	return (whole_size + 1);
}

char			*ft_strings_join(size_t args_size, char *delimiter, ...)
{
	va_list		args;
	va_list		copy;
	size_t		size;
	char		*joined;
	char		*str;

	va_start(args, delimiter);
	va_copy(copy, args);
	size = malloc_str(args_size, &copy, ft_strlen(delimiter), &joined);
	while (args_size-- && (str = va_arg(args, char *)))
	{
		if (ft_strlen(str) == 0)
			continue ;
		ft_strlcat(joined, str, size);
		if (ft_strlcat(joined, delimiter, size) > size)
			break ;
	}
	va_end(args);
	return (joined);
}

char			*ft_strarr_join(char *delimiter, char **array)
{
	int			i;
	size_t		whole_size;
	char		*str;

	whole_size = 0;
	i = 0;
	while (array[i])
		whole_size += ft_strlen(array[i++]);
	if (i == 0)
		return (ft_strdup(array[0]));
	whole_size += ft_strlen(delimiter) * (i - 1) + 1;
	str = ft_strnew(whole_size);
	i = -1;
	while (array[++i])
	{
		if (ft_strlen(array[i]) == 0)
			continue ;
		ft_strlcat(str, array[i], whole_size);
		if (ft_strlcat(str, delimiter, whole_size) > whole_size)
			break ;
	}
	return (str);
}
