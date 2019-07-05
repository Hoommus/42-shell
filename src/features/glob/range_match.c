/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   range_match.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 14:45:15 by mvladymy          #+#    #+#             */
/*   Updated: 2019/06/30 16:11:47 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdbool.h>
#include "ft_glob.h"

static inline bool	check_range(char ch, const char *range)
{
	bool	is_esq;

	is_esq = (*(range + 1) == '\\');
	return (ch >= *(range - 1) && ch <= *(range + 1 + is_esq));
}

static inline bool	end_range(size_t *range_len, size_t i,
								bool present, bool invert)
{
	if (range_len)
		*range_len += i;
	return (present != invert);
}

bool				range_match(char ch, const char *range, size_t *range_len)
{
	size_t	i;
	bool	invert;
	bool	present;

	invert = false;
	present = false;
	i = 0;
	while (!END_DIRNAME(range[++i]))
	{
		if (range[i] == '\\' && ++i && range[i] == ch)
			present = true;
		else if (range[i] == '-' && i > (size_t)(1 + invert)
			&& range[i + 1] != ']' && !END_DIRNAME(range[i + 1]))
		{
			if (check_range(ch, &range[i]))
				present = true;
		}
		else if (range[i] == '!' && i == 1)
			invert = true;
		else if (range[i] == ']' && i > (size_t)(1 + invert))
			return (end_range(range_len, i, present, invert));
		else if (range[i] == ch)
			present = true;
	}
	return (false);
}
