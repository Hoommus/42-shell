/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_glob.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/29 14:45:03 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 03:48:41 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GLOB_H
# define FT_GLOB_H

# include <stdbool.h>
# include <dirent.h>
# include <stddef.h>
# include <glob.h>
# include <limits.h>

# define GLOB_ONLYDIR (1 << 13)

int				ft_glob(const char *pattern,
						int flags,
						int (*errfunc) (const char *epath, int eerrno),
						glob_t *pglob);

void			ft_globfree(glob_t *pglob);

typedef struct	s_plist {
	char			*path;
	struct s_plist	*next;
}				t_plist;

t_plist			*add_path(t_plist **plist_p, char *path);
void			free_plist(t_plist *plist);
int				sort_paths(t_plist **plist_p);

int				create_pathv(glob_t *pglob, t_plist *plist, int flags);

# define END_DIRNAME(a) ({typeof(a) _a = (a); _a == '/' || _a == '\0';})

bool			range_match(char ch, const char *range, size_t *range_len);
bool			asterisk_match(const char *path, const char *pattern,
								size_t *path_i, size_t *pattern_i);
bool			dir_match(const char *path, const char *pattern,
							size_t *path_len, size_t *pattern_len);

typedef enum	e_dir_match {
	DIR_MATCH_NOMATCH = -1,
	DIR_MATCH_MATCH,
	DIR_MATCH_ISDIR,
	DIR_MATCH_ERROR
}				t_dir_match;

typedef struct	s_glob_stuff {
	char	path[PATH_MAX];
	int		flags;
	int		(*errfunc) (const char *epath, int eerrno);
}				t_glob_stuff;

int				parse_dirs(t_glob_stuff	*stuff, DIR *dir,
							const char *pattern, t_plist **plist);

#endif
