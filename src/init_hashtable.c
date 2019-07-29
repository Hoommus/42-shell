/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hashtable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 12:46:16 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/29 17:32:43 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_hashtable.h"

t_hash_tab	*g_hash_table;

#ifdef __linux__

void		populate_from_dir(const char *const dirpath, DIR *dir)
{
	struct dirent	*dirent;
	struct stat		s;
	char			path[2048];
	size_t			prefix_len;
	char			*swap;

	prefix_len = ft_strlen(dirpath) + 1;
	ft_bzero(path, sizeof(path));
	ft_strcpy(path, dirpath);
	ft_strcat(path, "/");
	while ((dirent = readdir(dir)))
	{
		ft_strcpy(path + prefix_len, dirent->d_name);
		*(path + prefix_len + ft_strlen(dirent->d_name)) = '\0';
		if ((dirent->d_type & (DT_REG | DT_LNK)) &&
			!stat(path, &s) &&
			!access(path, F_OK | X_OK))
		{
			swap = ft_strdup(dirent->d_name);
			hash_add(g_hash_table, swap, path);
			free(swap);
		}
	}
}

void		init_hashtable(void)
{
	size_t	i;
	t_var	*var;
	char	**split;
	DIR		*dir;

	hash_init(&g_hash_table, 1800);
	var = get_env_v(g_term->context_current->environ, "PATH");
	if (!var || !var->value || ft_strlen(var->value) == 0)
		return ;
	i = 0;
	split = NULL;
	split = ft_strsplit(var->value, ':');
	while (split && split[i])
	{
		dir = NULL;
		if (split[i] && split[i][0] && access(split[i], R_OK) == 0 &&
			(dir = opendir((const char *)split[i])))
		{
			populate_from_dir(split[i], dir);
			closedir(dir);
		}
		i++;
	}
	free_array((void **)split);
}

#endif

#ifdef __APPLE__

void		populate_from_dir(const char *const dirpath, DIR *dir)
{
	struct dirent	*dirent;
	struct stat		s;
	char			path[2048];
	size_t			prefix_len;
	char			*swap;

	prefix_len = ft_strlen(dirpath) + 1;
	ft_bzero(path, sizeof(path));
	ft_strcpy(path, dirpath);
	ft_strcat(path, "/");
	while ((dirent = readdir(dir)))
	{
		ft_strcpy(path + prefix_len, dirent->d_name);
		*(path + prefix_len + dirent->d_namlen) = '\0';
		if ((dirent->d_type & (DT_REG | DT_LNK)) &&
			!stat(path, &s) &&
			!access(path, F_OK | X_OK))
		{
			swap = ft_strdup(dirent->d_name);
			hash_add(g_hash_table, swap, path);
			free(swap);
		}
	}
}

void		init_hashtable(void)
{
	size_t	i;
	t_var	*var;
	char	**split;
	DIR		*dir;

	hash_init(&g_hash_table, 1800);
	var = get_env_v(g_term->context_current->environ, "PATH");
	if (!var || !var->value || ft_strlen(var->value) == 0)
		return ;
	i = 0;
	split = NULL;
	split = ft_strsplit(var->value, ':');
	while (split && split[i])
	{
		dir = NULL;
		if (split[i] && split[i][0] && (dir = opendir(split[i])))
		{
			populate_from_dir(split[i], dir);
			closedir(dir);
		}
		i++;
	}
	free_array((void **)split);
}

#endif
