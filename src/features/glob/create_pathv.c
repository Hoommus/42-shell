/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pathv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 14:51:22 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 13:51:28 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <glob.h>
#include <stddef.h>
#include <stdlib.h>
#include "libft.h"
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

int						create_pathv(glob_t *pglob, t_plist *plist, int flags)
{
	size_t	pathi;
	size_t	pathc;
	char	**pathv;

	pglob->gl_offs = (flags & GLOB_DOOFFS) ? pglob->gl_offs : 0;
	pglob->gl_pathc = (flags & GLOB_APPEND) ? pglob->gl_pathc : 0;
	pglob->gl_pathv = (flags & GLOB_APPEND) ? pglob->gl_pathv : NULL;
	pathc = get_path_num(plist) + pglob->gl_pathc;
	pathv = (char **)ft_memalloc(sizeof(char *) * (pathc + pglob->gl_offs + 1));
	if (!pathv)
		return (-1);
	pathi = pglob->gl_pathc + pglob->gl_offs;
	if (pglob->gl_pathv)
		ft_memcpy(pathv, pglob->gl_pathv, sizeof(char *) * pathi);
	while (plist)
	{
		pathv[pathi++] = plist->path;
		plist->path = NULL;
		plist = plist->next;
	}
	if (pglob->gl_pathv)
		free(pglob->gl_pathv);
	pglob->gl_pathv = pathv;
	pglob->gl_pathc = pathc;
	return (0);
}
