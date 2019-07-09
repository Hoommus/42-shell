/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter_paths.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/09 03:59:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"
#include "acompl.h"

static char	get_next_same_char(char *pathv[])
{
	static char		**spathv;
	static size_t	chi;
	char			next_char;
	size_t			pathvi;

	if (pathv != spathv)
	{
		spathv = pathv;
		chi = 0;
	}
	pathvi = 0;
	while (spathv[pathvi])
	{
		if (pathvi == 0)
			next_char = spathv[pathvi][chi];
		else
		{
			if (spathv[pathvi][chi] != next_char)
				return ('\0');
		}
		pathvi++;
	}
	chi++;
	return (next_char);
}

static bool	is_magic(char ch)
{
	return (ch == '\\' || ch == '!' || ch == '['
			|| ch == ']' || ch == '?' || ch == '*');
}

int			filter_paths(char *pathv[], char *result_buf, size_t res_size)
{
	size_t	i;
	char	next_char;

	if (!pathv[0])
		return (ACOMPL_NOTHING);
	else
	{
		i = 0;
		while ((next_char = get_next_same_char(pathv)) && i < res_size)
		{
			if (is_magic(next_char))
				result_buf[i++] = '\\';
			result_buf[i++] = next_char;
		}
		result_buf[i] = '\0';
		return ((pathv[1]) ? ACOMPL_CONTINUE : ACOMPL_END);
	}
	return (ACOMPL_ERROR);
}
