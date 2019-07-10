/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rn_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 18:55:05 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/10 12:16:54 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_builtins.h"

char			g_pwd[PATH_MAX];

static void		set_pwd_vars(void)
{
	char		old_pwd[PATH_MAX];
	const char	*env_pwd;

	old_pwd[0] = '\0';
	if ((env_pwd = get_env_var_ad("PWD")))
		ft_strlcat(old_pwd, env_pwd, PATH_MAX);
	set_env_var_ad("PWD", g_pwd);
	if (old_pwd[0])
		set_env_var_ad("OLDPWD", old_pwd);
}

static t_path	check_path(char *path)
{
	struct stat	file_stat;

	stat(path, &file_stat);
	if (access(path, F_OK))
		return (PATH_NOEXIST);
	else if (!S_ISDIR(file_stat.st_mode))
		return (PATH_NOTDIR);
	else if (access(path, X_OK))
		return (PATH_NOPERM);
	return (PATH_OK);
}

static	void	print_cd_error(t_path status, char *path)
{
	if (status == PATH_NOEXIST)
		ft_dprintf(STDERR_FILENO, "cd: no such file or directory: %s\n", path);
	else if (status == PATH_NOTDIR)
		ft_dprintf(STDERR_FILENO,
			"cd: not a directory: %s\n", path);
	else if (status == PATH_NOPERM)
		ft_dprintf(STDERR_FILENO,
			"cd: permission denied: %s\n", path);
}

static int		subst_path(char **path, char *arg)
{
	const char	*arg_path;

	if (!arg)
	{
		if (!(arg_path = get_env_var_ad("HOME")))
		{
			ft_dprintf(STDERR_FILENO, "cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strequ(arg, "-"))
	{
		if (!(arg_path = get_env_var_ad("OLDPWD")))
		{
			ft_dprintf(STDERR_FILENO, "cd: OLDWPD not set\n");
			return (1);
		}
		ft_printf("%s\n", arg_path);
	}
	else
		arg_path = arg;
	return (!(*path = make_logic_path(arg_path)));
}

int				rn_cd(const char **argv)
{
	char	*path;
	t_path	path_status;
	bool	logic;

	if (!~is_logic((char ***)&argv, &logic, "cd") ||
		subst_path(&path, (char *)*argv))
		return (1);
	if ((path_status = check_path(path)) == PATH_OK)
	{
		chdir(path);
		if (logic)
			ft_strncpy(g_pwd, path, PATH_MAX);
		else
			getcwd(g_pwd, PATH_MAX);
		set_pwd_vars();
	}
	else
		print_cd_error(path_status,
			((*argv && !ft_strequ(*argv, "-")) ? (char *)*argv : path));
	if (path)
		free(path);
	return (0);
}
