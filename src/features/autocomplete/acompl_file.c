/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 16:54:45 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/stat.h>
#include "libft.h"
#include "ft_glob.h"
#include "acompl.h"

static char		*create_pattern(char *str)
{
	size_t	pat_len;
	char	*pattern;

	pat_len = ft_strlen(str) + 2;
	if (!pat_len || !(pattern = (char *)malloc(sizeof(char) * pat_len)))
		return (NULL);
	pattern[0] = '\0';
	ft_strlcat(pattern, str, pat_len);
	ft_strlcat(pattern, "*", pat_len);
	return (pattern);
}

static size_t	count_suitable_paths(char *pathv[])
{
	size_t		pathi;
	size_t		suiti;
	struct stat	sstat;

	suiti = 0;
	pathi = 0;
	while (pathv[pathi])
	{
		if (stat(pathv[pathi], &sstat))
			return (-1);
		if (S_ISDIR(sstat.st_mode) || (S_IXUSR & sstat.st_mode
				&& ft_strchr(pathv[pathi], '/')))
			suiti++;
		pathi++;
	}
	return (suiti);
}

static void		set_pathv(char *old[], char *new[])
{
	size_t		oldi;
	size_t		newi;
	struct stat	sstat;

	oldi = 0;
	newi = 0;
	while (old[oldi])
	{
		stat(old[oldi], &sstat);
		if (S_ISDIR(sstat.st_mode) || (S_IXUSR & sstat.st_mode
				&& ft_strchr(old[oldi], '/')))
			new[newi++] = old[oldi];
		else
			free(old[oldi]);
		oldi++;
	}
	new[newi] = NULL;
}

static int		del_common_files(glob_t *pglob)
{
	size_t		rem_pathc;
	char		**rem_pathv;

	rem_pathc = count_suitable_paths(&pglob->gl_pathv[pglob->gl_offs]);
	if (!(rem_pathv = (char **)malloc(
			sizeof(char *) * (rem_pathc + pglob->gl_offs + 1))))
		return (-1);
	memset(rem_pathv, 0, pglob->gl_offs);
	set_pathv(&pglob->gl_pathv[pglob->gl_offs], rem_pathv);
	free(pglob->gl_pathv);
	pglob->gl_pathc = rem_pathc;
	pglob->gl_pathv = rem_pathv;
	return (0);
}

int				acompl_file(char *input_str, char *result_buf,
							size_t res_size, bool is_cmd)
{
	glob_t	gl;
	int		res;
	char	*pattern;

	if (!(pattern = create_pattern(input_str)))
		return (ACOMPL_ERROR);
	ft_glob(pattern, GLOB_MARK, NULL, &gl);
	if (is_cmd && del_common_files(&gl))
		res = ACOMPL_ERROR;
	else
		res = filter_paths(&gl.gl_pathv[gl.gl_offs], result_buf, res_size);
	free(pattern);
	ft_globfree(&gl);
	return (res);
}
