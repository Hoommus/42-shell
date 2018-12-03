#include "twenty_one_sh.h"

int		get_history_fd(void)
{
	if (fcntl(g_term->history_file, F_GETFD) == -1)
		g_term->history_file = init_fd_at_home(HISTORY_FILE, 0);
	return (g_term->history_file);
}
