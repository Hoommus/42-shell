#include "../include/minishell.h"

void	ignore(int sig)
{
	sig = 0;
	shell_loop();
}

void	setup_signal_handlers(void)
{
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, &ignore);
}
