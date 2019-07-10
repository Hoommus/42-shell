/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rn_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 20:41:38 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/10 12:11:24 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

extern char	g_pwd[PATH_MAX];

int			rn_pwd(const char **argv)
{
	bool	logic;
	char	pwd[PATH_MAX];

	if (is_logic((char ***)&argv, &logic, "pwd"))
		return (1);
	if ((*argv && ft_strcmp(*argv, "-")) ||( *argv && *(argv + 1)))
	{
		ft_dprintf(STDERR_FILENO, "pwd: too many arguments\n");
		return (1);
	}
	if (!ft_strlen(g_pwd))
		getcwd(g_pwd, PATH_MAX);
	if (logic)
		ft_strncpy(pwd, g_pwd, PATH_MAX - 1);
	else
		getcwd(pwd, PATH_MAX);
	pwd[PATH_MAX - 1] = '\0';
	ft_printf("%s\n", pwd);
	return (0);
}
