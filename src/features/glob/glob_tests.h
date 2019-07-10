/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_tests.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 13:29:44 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 14:28:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOB_TESTS_H
# define GLOB_TESTS_H

# include <glob.h>
# include <dirent.h>
# include <stdlib.h>
# include <stdbool.h>
# include <dirent.h>
# include <sys/types.h>
# include <limits.h>

# include "ft_glob.h"

# include <locale.h>
# include <stdio.h>
# include <string.h>

void	glob_match_test(char *path, char *pattern, bool assert);
void	range_test_match(char ch, char *pattern, bool assert);
void	glob_test(char *pattern, size_t offset, int flags);

void	range(void);
void	range2(void);

void	match(void);
void	match2(void);
void	match3(void);

#endif
