/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 14:45:11 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 03:50:29 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <glob.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <limits.h>
#include "libft.h"
#include "ft_glob.h"

/*
** Implemented flags:
** - GLOB_NOSORT
** - GLOB_ERR
** - GLOB_DOOFFS
** - GLOB_MARK
** - GLOB_ONLYDIR
*/

int		ft_glob(const char *pattern,
				int flags,
				int (*errfunc) (const char *epath, int eerrno),
				glob_t *pglob)
{
	t_glob_stuff	stuff;
	DIR				*dir;
	int				return_code;
	t_plist			*plist;

	if (!(dir = opendir((pattern[0] == '/') ? "/" : ".")))
		return ((flags & GLOB_ERR) ? GLOB_ABORTED : 0);
	stuff.path[0] = '\0';
	stuff.flags = flags;
	stuff.errfunc = errfunc;
	if (pattern[0] == '/')
		ft_strlcat(stuff.path, "/", PATH_MAX);
	plist = NULL;
	if (!(return_code = parse_dirs(&stuff, dir, pattern, &plist)))
	{
		if (!(flags & GLOB_NOSORT))
			sort_paths(&plist);
		if (create_pathv(pglob, plist, flags))
			return_code = GLOB_NOSPACE;
	}
	free_plist(plist);
	closedir(dir);
	return ((pglob->gl_pathc) ? return_code : GLOB_NOMATCH);
}

void	ft_globfree(glob_t *pglob)
{
	size_t	gl_pathi;

	gl_pathi = 0;
	while (gl_pathi < pglob->gl_pathc + pglob->gl_offs)
	{
		if (pglob->gl_pathv[gl_pathi])
			free(pglob->gl_pathv[gl_pathi]);
		gl_pathi++;
	}
	free(pglob->gl_pathv);
	pglob->gl_pathv = NULL;
	pglob->gl_pathc = 0;
	pglob->gl_offs = 0;
}
