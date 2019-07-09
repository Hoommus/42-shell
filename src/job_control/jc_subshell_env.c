/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_subshell_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 04:03:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/09 04:03:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

static bool		g_is_subhsell_env;
static pid_t	g_who_locked;
static bool		g_state_lock;

int		jc_lock_subshell_state(pid_t pgid)
{
	if (g_who_locked == 0)
		g_who_locked = pgid;
	if (g_who_locked != pgid)
		return (false);
	if (!g_state_lock)
	{
		g_state_lock = true;
		return (true);
	}
	return (false);
}

int		jc_unlock_subshell_state(pid_t pgid)
{
	if (pgid != g_who_locked)
		return (false);
	g_state_lock = false;
	g_who_locked = 0;
	return (true);
}

int		jc_enable_subshell(void)
{
	if (!g_state_lock && !g_is_subhsell_env)
		return (g_is_subhsell_env = true);
	else
		return (false);
}

int		jc_disable_subshell(void)
{
	if (!g_state_lock && g_is_subhsell_env)
		return (g_is_subhsell_env = false);
	else
		return (false);
}

int		jc_is_subshell(void)
{
	return (g_is_subhsell_env);
}
