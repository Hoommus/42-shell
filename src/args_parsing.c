/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:27:57 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/19 16:55:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** Returns 0 if all's good and specific flag if it is invalid
*/

char	validate_short_flags(const char **args, const char *possible_flags)
{
	int		i;
	int		k;

	i = -1;
	while (args && args[++i] && ft_strncmp(args[i], "-", 1) == 0 && !(k = 0))
		while (args[i][++k])
			if (ft_strchr(possible_flags, args[i][k]) == NULL)
				return (args[i][k]);
	return (0);
}

/*
** TODO: add '--' check so loop doesn't stop
*/

bool	has_long_flag(const char **args, const char *flag)
{
	bool	status;
	int		i;

	i = 0;
	status = false;
	while (args && args[i] && !status && ft_strncmp(args[i], "--", 2) == 0)
		status = !!(ft_strcmp(args[i++], flag) == 0);
	return (status);

}

bool	has_flag(const char **args, const char flag)
{
	bool	status;
	char	*copy;
	int		i;

	i = 0;
	status = false;
	while (args && args[i] && !status && ft_strncmp(args[i], "-", 1) == 0)
	{
		copy = (char *)args[i++];
		while (*copy && !status)
			if (*copy == flag)
				status = true;
			else
				copy++;
	}
	return (status);
}
