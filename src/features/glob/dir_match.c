/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_match.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 16:16:27 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/02 23:57:56 by reineke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include "ft_glob.h"

#define SET_IF_EXIST(x, y) ((x) ? (*(x) = (y)) : ((void)0))
#define ADD_IF_EXIST(x, y) ((x) ? (*(x) += (y)) : ((void)0))

#define IS_FIRST_DOT(path, path_i) (path_i == 0 && path[path_i] == '.')

static inline bool	char_match(const char *path, const char *pattern,\
								size_t *path_i, size_t *pattr_i)
{
	bool	esq;

	if ((esq = (pattern[*pattr_i] == '\\')))
		(*pattr_i)++;
	if ((!esq && !IS_FIRST_DOT(path, *path_i) &&
			(pattern[*pattr_i] == '?' || (pattern[*pattr_i] == '[' &&
			range_match(path[*path_i], &pattern[*pattr_i], pattr_i)))) ||
			path[*path_i] == pattern[*pattr_i])
	{
		(*path_i)++;
		(*pattr_i)++;
	}
	else if (pattern[*pattr_i] == '*' && !esq && !IS_FIRST_DOT(path, *path_i)
			&& asterisk_match(&path[*path_i],
			&pattern[*pattr_i], path_i, pattr_i))
		;
	else
		return (false);
	return (true);
}

bool				dir_match(const char *path, const char *pattern,
								size_t *path_len, size_t *pattern_len)
{
	size_t	path_i;
	size_t	pattr_i;

	path_i = 0;
	pattr_i = 0;
	while (!END_DIRNAME(pattern[pattr_i]))
	{
		if (!char_match(path, pattern, &path_i, &pattr_i))
			break ;
	}
	if (END_DIRNAME(path[path_i]) && END_DIRNAME(pattern[pattr_i]))
	{
		SET_IF_EXIST(path_len, path_i);
		SET_IF_EXIST(pattern_len, pattr_i);
		return (true);
	}
	return (false);
}
