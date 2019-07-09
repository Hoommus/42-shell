/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hs_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 12:47:30 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/05 12:47:30 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "twenty_one_sh.h"
#include "shell_builtins.h"

int					hs_exit(const char **args)
{
	if (args && *args)
	{
		jc_check_n_notify(false);
		if (!is_string_numeric(*args, 10) || args[1])
		{
			ft_dprintf(2, SH ": exit: %s: single numeric argument required\n",
				*args);
			if (jc_is_subshell())
				return (2);
			else
				exit(2);
		}
		else if (jc_is_subshell())
			return (ft_atoi(*args));
		else if (jc_get()->active_jobs != NULL)
			return ((ft_dprintf(2, "You have running jobs.\n") & 0) | 1);
		else
			exit(ft_atoi(*args));
	}
	if (jc_is_subshell())
		return (0);
	else if (jc_get()->active_jobs != NULL)
		return ((ft_dprintf(2, "You have running jobs.\n") & 0) | 1);
	else
		exit(0);
}
