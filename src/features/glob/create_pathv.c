/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pathv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 14:51:22 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 00:37:38 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <glob.h>
#include <stddef.h>
#include <stdlib.h>
#include "ft_glob.h"

static inline size_t	get_path_num(t_plist *plist)
{
	size_t	path_num;

	path_num = 0;
	while (plist)
	{
		path_num++;
		plist = plist->next;
	}
	return (path_num);
}

static inline void		set_offset(glob_t *pglob, size_t path_num)
{
	size_t	i;

	i = 0;
	while (i < path_num)
		pglob->gl_pathv[i++] = NULL;
}

int						create_pathv(glob_t *pglob, t_plist *plist, int flags)
{
	size_t	path_num;
	size_t	pathv_len;

	pglob->gl_pathc = get_path_num(plist);
	pathv_len = (flags & GLOB_DOOFFS) ? pglob->gl_pathc + pglob->gl_offs
									: pglob->gl_pathc;
	pglob->gl_offs = (flags & GLOB_DOOFFS) ? pglob->gl_offs : 0;
	if (!(pglob->gl_pathv = (char **)malloc(sizeof(char *) * (pathv_len + 1))))
		return (-1);
	path_num = (flags & GLOB_DOOFFS) ? pglob->gl_offs : 0;
	set_offset(pglob, path_num);
	while (plist)
	{
		pglob->gl_pathv[path_num++] = plist->path;
		plist->path = NULL;
		plist = plist->next;
	}
	pglob->gl_pathv[path_num] = NULL;
	return (0);
}
