/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 14:45:11 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 14:40:29 by mvladymy         ###   ########.fr       */
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
** - GLOB_APPEND // currentry doesn't work correctly with GLOB_DOOFFS flag
** - GLOB_NOCHECK
** - GLOB_NOMAGIC
*/

#define INIT_STUFF(s, f, e) {s.path[0] = '\0'; s.flags = f; s.errfunc = e;}

static bool	has_magic(const char *p)
{
	while (*p)
	{
		if (*p == '\\' || *p == '!' || *p == '['
				|| *p == ']' || *p == '?' || *p == '*')
			return (true);
		p++;
	}
	return (false);
}

static int	no_match_handler(t_plist **plist, const char *pattern, int flags)
{
	if (flags & GLOB_NOCHECK || (flags & GLOB_NOMAGIC && !has_magic(pattern)))
	{
		add_path(plist, pattern);	
		return (0);
	}
	else
		return (GLOB_NOMATCH);
}

int			ft_glob(const char *pattern,
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
	INIT_STUFF(stuff, flags, errfunc);
	if (pattern[0] == '/')
		ft_strlcat(stuff.path, "/", PATH_MAX);
	plist = NULL;
	if (!(return_code = parse_dirs(&stuff, dir, pattern, &plist)))
	{
		if (!plist)
			return_code = no_match_handler(&plist, pattern, flags);
		if (!(flags & GLOB_NOSORT))
			sort_paths(&plist);
		if (create_pathv(pglob, plist, flags))
			return_code = GLOB_NOSPACE;
	}
	free_plist(plist);
	closedir(dir);
	return (return_code);
}

void		ft_globfree(glob_t *pglob)
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
