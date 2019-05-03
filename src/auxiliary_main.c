/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 15:32:39 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/03 16:03:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int					display_normal_prompt(void)
{
	t_var	*home;
	t_var	*user;
	char	host[1025];
	char	cwd[1025];
	int		size;

	home = get_env_v(NULL, "HOME");
	getcwd(cwd, 1024);
	gethostname(host, 1024);
	if (home && home->value && ft_strcmp(cwd, home->value) == 0 &&
		(cwd[ft_strlen(cwd)] == '/' || cwd[ft_strlen(cwd)] == 0))
		ft_strcpy(cwd, "~");
	host[ft_strchr(host, '.') - host] = 0;
	user = get_env_v(NULL, "USER");
	size = ft_printf(SHELL_PROMPT,
		user ? user->value : "$USER", host,
		ft_strrchr(cwd, '/') == NULL ? cwd
									: ft_strrchr(cwd, '/') + !!(cwd[1] != '\0'),
		g_term->last_status ? 31 : 32);
	return (size);
}
