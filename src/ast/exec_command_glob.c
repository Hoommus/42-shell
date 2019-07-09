/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_glob.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 20:47:55 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 17:37:43 by vtarasiu         ###   ########.fr       */
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
		ft_glob(command->args[i], GLOB_NOCHECK | GLOB_APPEND, &errfunc, &globt);
		i++;
	}
	free_array((void **)command->args);
	command->args = globt.gl_pathv;
}
