/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 20:03:54 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 03:49:15 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <glob.h>
#include <stddef.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "libft.h"
#include "ft_glob.h"

#define SKIP_SLASHES(a) while (*(a) == '/') (a)++

static t_dir_match	check_dir(struct dirent *dirent,
								const char *pattern,
								t_glob_stuff *stuff,
								size_t *patr_len)
{
	struct stat	sstat;

	if (!dir_match(dirent->d_name, pattern, NULL, patr_len))
		return (DIR_MATCH_NOMATCH);
	pattern += *patr_len;
	ft_strlcat(stuff->path, dirent->d_name, PATH_MAX);
	if (stat(stuff->path, &sstat))
		return (DIR_MATCH_ERROR);
	if (!S_ISDIR(sstat.st_mode) && stuff->flags & GLOB_ONLYDIR)
		return (DIR_MATCH_NOMATCH);
	if (!*pattern && !(S_ISDIR(sstat.st_mode) && stuff->flags & GLOB_MARK))
		return (DIR_MATCH_MATCH);
	SKIP_SLASHES(pattern);
	ft_strlcat(stuff->path, "/", PATH_MAX);
	if (!*pattern && S_ISDIR(sstat.st_mode))
		return (DIR_MATCH_MATCH);
	else if (S_ISDIR(sstat.st_mode))
		return (DIR_MATCH_ISDIR);
	return (DIR_MATCH_ERROR);
}

static int			dirent_dir_handler(t_glob_stuff *stuff,
										const char *pattern,
										t_plist **plist)
{
	DIR	*next_dir;
	int	return_code;

	if (!(next_dir = opendir(stuff->path)))
	{
		if (stuff->flags & GLOB_ERR)
			return (GLOB_ABORTED);
		else
			return (0);
	}
	return_code = parse_dirs(stuff, next_dir, pattern, plist);
	closedir(next_dir);
	if (return_code)
		return (return_code);
	return (0);
}

static int			dirent_handler(t_glob_stuff *stuff,
									struct dirent *dirent,
									const char *pattern,
									t_plist **plist)
{
	size_t			patr_len;
	t_dir_match		find_code;
	int				return_code;

	find_code = check_dir(dirent, pattern, stuff, &patr_len);
	if (find_code == DIR_MATCH_NOMATCH)
		return (0);
	else if (find_code == DIR_MATCH_MATCH)
	{
		if (!add_path(plist, stuff->path))
			return (GLOB_NOSPACE);
	}
	else if (find_code == DIR_MATCH_ERROR &&
			((stuff->errfunc && stuff->errfunc(stuff->path, errno)) ||
			stuff->flags & GLOB_ERR))
		return (GLOB_ABORTED);
	else if (find_code == DIR_MATCH_ISDIR &&
			(return_code = dirent_dir_handler(stuff,
				pattern + patr_len, plist)))
		return (return_code);
	return (0);
}

int					parse_dirs(t_glob_stuff *stuff,
								DIR *dir,
								const char *pattern,
								t_plist **plist)
{
	size_t			path_len;
	struct dirent	*dirent;
	int				return_code;

	SKIP_SLASHES(pattern);
	return_code = 0;
	path_len = strnlen(stuff->path, PATH_MAX);
	while ((dirent = readdir(dir)))
	{
		stuff->path[path_len] = '\0';
		if ((return_code = dirent_handler(stuff, dirent, pattern, plist)))
			break ;
	}
	return (return_code);
}
