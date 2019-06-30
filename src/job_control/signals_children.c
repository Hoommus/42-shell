
#include "shell_job_control.h"
#include "twenty_one_sh.h"



void		sigchild_set_handler(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(struct sigaction));
	action.sa_flags = SA_SIGINFO;
	action.sa_sigaction = &sigchild;
	sigaction(SIGCHLD, &action, NULL);
}