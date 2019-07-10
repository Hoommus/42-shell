/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rn_cd_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 17:46:44 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/10 12:21:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

extern char		g_pwd[PATH_MAX];

static size_t	add_dir(const char *src, char *dst, size_t max_len)
{
	size_t	i;
	size_t	src_len;

	i = 0;
	src_len = ft_strlen(src);
	dst[i++] = '/';
	while (i < max_len - 2 && i - 1 < src_len && src[i - 1] != '/')
	{
		dst[i] = src[i - 1];
		i++;
	}
	dst[i] = '\0';
	return (i);
}

static size_t	del_dir(char *path, size_t pos)
{
	size_t	i;

	i = 0;
	while (pos - i > 0 && path[pos - i] != '/')
		i++;
	path[(pos > i) ? pos - i : pos - i + 1] = '\0';
	return (i);
}

static size_t	sobaka(const char *src, char *dst, size_t *d_i, size_t max_len)
{
	size_t	tmp;

	if (src[0] == '.' && (src[1] == '/' || src[1] == '\0'))
		return (2);
	else if (src[0] == '.' && src[1] == '.' &&
			 (src[2] == '/' || src[2] == '\0'))
	{
		*d_i -= del_dir(dst, *d_i);
		return (3);
	}
	else if (src[0] == '/')
		return (1);
	else
	{
		tmp = add_dir(src, &dst[*d_i], max_len - *d_i);
		*d_i += tmp;
		return (tmp);
	}
}

static void		copy_path(const char *src, char *dst, size_t max_len)
{
	size_t	s_i;
	size_t	d_i;

	d_i = 0;
	while (d_i < max_len && dst[d_i])
		d_i++;
	s_i = 0;
	while (d_i < max_len && s_i < ft_strlen(src))
		s_i += sobaka(&src[s_i], dst, &d_i, max_len);
	if (!d_i)
		ft_strcpy(dst, "/");
}

char			*make_logic_path(const char *path)
{
	char	path_buff[PATH_MAX];

	path_buff[0] = '\0';
	if (path[0] != '/')
	{
		if (!ft_strlen(g_pwd))
			getcwd(g_pwd, PATH_MAX);
		copy_path(g_pwd, path_buff, PATH_MAX);
	}
	copy_path(path, path_buff, PATH_MAX);
	return (ft_strdup(path_buff));
}
