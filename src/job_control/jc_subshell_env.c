#include "shell_job_control.h"

static bool		g_is_subhsell_env;
static pid_t	who_locked;
static bool		g_state_lock;

int		jc_lock_subshell_state(pid_t pgid)
{
	if (who_locked == 0)
		who_locked = pgid;
	if (who_locked != pgid)
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
	if (pgid != who_locked)
		return (false);
	g_state_lock = false;
	who_locked = 0;
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
