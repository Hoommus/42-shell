/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_glob.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 20:47:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/05 14:39:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script_parser.h"
#include "twenty_one_sh.h"
#include "ft_glob.h"

int		errfunc(const char *str, int a)
{
	str = NULL;
	return (a = 0);
}

void	expand_globs(struct s_command *command)
{
	glob_t		globt;
	int			i;

	ft_bzero(&globt, sizeof(globt));
	i = 0;
	while (command->args[i])
	{
		if ((glob(command->args[i], GLOB_NOCHECK | GLOB_APPEND,
			&errfunc, &globt)) == GLOB_NOCHECK)
		{
			globt.gl_pathv[globt.gl_pathc] = command->args[i];
			globt.gl_pathc++;
		}
		i++;
	}
	free_array((void **)command->args);
	command->args = globt.gl_pathv;
}
