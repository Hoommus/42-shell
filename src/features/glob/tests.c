/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 17:13:49 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/10 14:29:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <glob.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <limits.h>

#include "ft_glob.h"
#include "glob_tests.h"

#include <locale.h>
#include <stdio.h>
#include <string.h>

void	glob_test(char *pattern, size_t offset, int flags)
{
	glob_t	gl;
	int		ret;
	size_t	i;

	gl.gl_offs = offset;
	ret = ft_glob(pattern, flags, 0, &gl);
	printf("offset: %zu path number: %zu\n", gl.gl_offs, gl.gl_pathc);
	i = 0;
	while (i < gl.gl_offs + gl.gl_pathc)
		printf("\t%s\n", gl.gl_pathv[i++]);
	printf("return code: %d\n", ret);
	ft_globfree(&gl);
}

void	range_test_match(char ch, char *pattern, bool assert)
{
	size_t	a;
	bool	find;

	a = 0;
	find = range_match(ch, pattern, &a);
	printf("%lc %s %zu (`%c' in \"%s\")\n", ((assert == find) ? L'✅' : L'❌'),
											((find) ? "true " : "false"),
											a, ch, pattern);
}

void	glob_match_test(char *path, char *pattern, bool assert)
{
	bool	match;

	match = dir_match(path, pattern, 0, 0);
	printf("%lc %s (\"%s\" and \"%s\")\n", ((assert == match) ? L'✅' : L'❌'),
											((match) ? "true " : "false"),
											path, pattern);
}

int		main(void)
{
	setlocale(LC_ALL, "");
	range();
	range2();
	match();
	match2();
	match3();
}
