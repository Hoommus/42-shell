
#include "shell_job_control.h"
#include "twenty_one_sh.h"

static void		sigchild_handler(int sig, siginfo_t *info, void *ignore)
{
	pid_t		pgid;
	const t_job	*jobs = jc_get()->active_jobs;
	t_proc		*procs;

	sig = 0;
	ignore = 0;
	pgid = getpgid(info->si_pid);
	while (jobs)
	{
		procs = jobs->procs;
		while (procs && jobs->pgid == pgid)
		{
			if (procs->pid == info->si_pid)
			{
				waitpid(procs->pid, &procs->status, WUNTRACED);
				alterate_proc((t_job *)jobs, procs);
				break ;
			}
			procs = procs->next;
		}
		jobs = jobs->next;
	}
	tcsetattr(0, TCSADRAIN, g_term->shell_term);
}

void			sigchild_set_handler(void)
{
	sigaction(SIGCHLD, &((struct sigaction){
		.sa_mask = 0,
		.sa_flags = SA_SIGINFO,
		.sa_sigaction = &sigchild_handler
	}), NULL);
}

void			unset_signal_handlers(void)
{
	const static struct sigaction	act;

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGQUIT, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);
	sigaction(SIGCHLD, &act, NULL);
	sigaction(SIGWINCH, &act, NULL);
}
