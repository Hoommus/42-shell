/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/03 23:50:38 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "acompl.h"

const char	*g_builtins_sample[] = {
	"foo",
	"bar",
	"fofobar",
	"fibar",
	"sobaka",
	NULL
};

const char *g_alias_sample[] = {
	"lol",
	"ololo",
	"kek",
	"lolkek",
	NULL
};

static int	set_pathv(char *input_str, char *pathv[])
{
	size_t	input_len;
	size_t	path_count;
	size_t	i;

	input_len = ft_strlen(input_str);
	path_count = 0;
	i = 0;
	while (g_builtins_sample[i])
	{
		if (ft_strnstr(g_builtins_sample[i], input_str, input_len))
			pathv[path_count++] = ft_strdup(g_builtins_sample[i]);
		i++;
	}
	pathv[path_count] = NULL;
	return (0);
}

static int	get_path_count(char *input_str)
{
	size_t	input_len;
	size_t	path_count;
	size_t	i;

	input_len = ft_strlen(input_str);
	path_count = 0;
	i = 0;
	while (g_builtins_sample[i])
	{
		if (ft_strnstr(g_builtins_sample[i], input_str, input_len))
			path_count++;
		i++;
	}
	return (path_count);
}

static void	free_pathv(char *pathv[])
{
	size_t	i;

	i = 0;
	while (pathv[i])
		free(pathv[i++]);
	free(pathv);
}

static int	get_pathv(char *input_str, char **pathv[])
{
	size_t	pathc;

	pathc = get_path_count(input_str);
	if (!(*pathv = (char **)malloc(sizeof(char **) * (pathc + 1))))
		return (-1);
	if (set_pathv(input_str, *pathv))
	{
		free_pathv(*pathv);
		return (-1);
	}
	return (0);
}

int			acompl_cmd(char *input_str, char *result_buf, size_t res_size)
{
	int		ret;
	char	**pathv;

	if (get_pathv(input_str, &pathv))
		return (ACOMPL_ERROR);
	ret = filter_paths(pathv, result_buf, res_size);
	free_pathv(pathv);
	return (ret);
}
