/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asterisk_match.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 17:43:06 by mvladymy          #+#    #+#             */
/*   Updated: 2019/06/30 18:59:50 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include "ft_glob.h"

bool	asterisk_match(const char *path, const char *pattern,
						size_t *path_i, size_t *pattern_i)
{
	size_t	star_eats;
	size_t	pattern_size;
	size_t	recur_buf;

	star_eats = 0;
	pattern_size = 0;
	while (!END_DIRNAME(path[star_eats]))
		star_eats++;
	if (END_DIRNAME(pattern[++pattern_size]))
	{
		*path_i += star_eats;
		*pattern_i += pattern_size;
		return (true);
	}
	while (star_eats--)
	{
		if (dir_match(&path[star_eats], &pattern[pattern_size],
				&recur_buf, &pattern_size))
		{
			*path_i += star_eats + recur_buf;
			*pattern_i += pattern_size + 1;
			return (true);
		}
	}
	return (false);
}
