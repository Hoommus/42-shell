/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filter_paths.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/04 20:12:13 by vtarasiu         ###   ########.fr       */
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

int			filter_paths(char *pathv[], char *result_buf, size_t res_size)
{
	size_t	i;

	if (!pathv[0])
		return (ACOMPL_NOTHING);
	else if (!pathv[1])
	{
		ft_strncpy(result_buf, pathv[0], res_size - 1);
		result_buf[res_size - 1] = '\0';
		return (ACOMPL_END);
	}
	else
	{
		i = 0;
		while ((result_buf[i++] = get_next_same_char(pathv)))
			;
		return (ACOMPL_CONTINUE);
	}
	return (ACOMPL_ERROR);
}
