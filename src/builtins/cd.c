/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:58 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/10 12:33:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int		try_access_and_cd(const char *path)
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

static int		try_cd(const char *path, t_var *oldpwd)
{
	t_var	*home;
	int		status;

	home = get_env_v(NULL, "HOME");
	if (path == NULL || ft_strcmp(path, "--") == 0 || ft_strlen(path) == 0)
	{
		if (home && home->value)
			status = try_access_and_cd(home->value);
		else
			return (0);
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		if (oldpwd == NULL)
			return ((ft_dprintf(2, ANSI_RESET "cd: OLDPWD not set\n") & 0) | 2);
		ft_printf("%s\n", oldpwd->value);
		status = try_access_and_cd(oldpwd->value);
	}
	else
		status = try_access_and_cd(path);
	return (status);
}

int				hs_cd(const char **args)
{
	const t_env_vector	*vector = g_term->context_original->environ;
	char				currpwd[1024];
	char				pwd[1024];
	int					status;

	getcwd(currpwd, 1023);
	status = try_cd(args[0], get_env_v(NULL, "OLDPWD"));
	if (status == 0)
	{
		getcwd(pwd, 1023);
		set_env_v((t_env_vector *)vector, "PWD", pwd, SCOPE_EXPORT);
		set_env_v((t_env_vector *)vector, "OLDPWD", currpwd, SCOPE_EXPORT);
		return (0);
	}
	else if (status == 1348)
		return ((ft_dprintf(2, "cd: permission denied: %s\n", *args) & 0) | 1);
	else if (status == 1)
		ft_dprintf(2, "cd: no such file or directory: %s\n", args[0]);
	return (status);
}
