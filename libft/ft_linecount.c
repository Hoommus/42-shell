/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_linecount.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/18 19:06:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/19 14:15:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Counts how many lines will this string take up if fit into a viewport of
** line_width characters wide.
*/

size_t	linecount(const char *str, int64_t line_width)
{
	int64_t		i;
	int64_t		part_len;
	size_t		lines;

	if (line_width <= 0)
		line_width = INT64_MAX;
	if (line_width == 1)
		return (ft_utf_strlen((char *)str));
	i = 0;
	lines = 0;
	part_len = 0;
	while (str[i])
	{
		part_len++;
		if (str[i] == '\n')
		{
			lines += part_len / line_width + part_len % line_width == 0 ? 0 : 1;
			part_len = 0;
		}
		i++;
	}
	if (part_len != 0)
		lines += part_len / line_width;
	return (lines);
}
