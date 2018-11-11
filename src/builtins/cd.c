/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:58 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/08/01 12:43:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int				try_access_and_cd(char *path)
{
	int		status;

	status = 1;
	if (path != NULL && access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			status = chdir(path);
		else
			status = 1348;
	}
	return (status);
}

int				try_cd(char *path, char *oldpwd)
{
	int		status;
	char	*home;

	home = get_env("HOME");
	if (path == NULL || ft_strcmp(path, "--") == 0 || ft_strlen(path) == 0)
	{
		if (home != NULL)
			status = try_access_and_cd(home);
		else
			return (0);
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		if (oldpwd == NULL)
			oldpwd = home;
		ft_printf("%s\n", oldpwd);
		status = try_access_and_cd(oldpwd);
	}
	else
		status = try_access_and_cd(path);
	return (status);
}

int				hs_cd(char **args)
{
	char		*oldpwd;
	char		currpwd[1024];
	char		pwd[1024];
	int			status;

	oldpwd = get_env("OLDPWD");
	getcwd(currpwd, 1024);
	status = try_cd(args[0], oldpwd);
	if (status == 0)
	{
		getcwd(pwd, 1024);
		set_env("PWD", pwd);
		set_env("OLDPWD", currpwd);
	}
	else if (status == 1348)
		ft_dprintf(2, "cd: permission denied: %s\n", args[0]);
	else
		ft_dprintf(2, "cd: no such file or directory: %s\n", args[0]);
	return (0);
}
