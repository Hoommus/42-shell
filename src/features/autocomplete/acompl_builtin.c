/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   acompl_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvladymy <mvladymy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 03:17:00 by mvladymy          #+#    #+#             */
/*   Updated: 2019/07/06 19:46:33 by mvladymy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "acompl.h"
#include "shell_builtins.h"

extern struct s_builtin	g_builtins[];

int						get_builtv(char *input_str, char *builtv[])
{
	size_t	input_len;
	size_t	builti;
	size_t	i;

	input_len = ft_strlen(input_str);
	builti = 0;
	i = 0;
	while (g_builtins[i].name)
	{
		if (ft_strnstr(g_builtins[i].name, input_str, input_len))
			builtv[builti++] = ft_strdup(g_builtins[i].name);
		i++;
	}
	builtv[builti] = NULL;
	return (0);
}

int						get_builtc(char *input_str)
{
	size_t	input_len;
	size_t	builtc;
	size_t	i;

	input_len = ft_strlen(input_str);
	builtc = 0;
	i = 0;
	while (g_builtins[i].name)
	{
		if (ft_strnstr(g_builtins[i].name, input_str, input_len))
			builtc++;
		i++;
	}
	return (builtc);
}
