
#include "shell_job_control.h"

void		jc_to_bg(t_job *job)
{
	t_proc	*segments;

	job->state = JOB_CONTINUED;
	jc_format_job(job);
	segments = job->procs;
	while (segments)
	{
		if (segments->is_stopped && !segments->is_completed)
		{
			kill(segments->pid, SIGCONT);
			segments->is_stopped = false;
			job->notified = false;
		}
		segments = segments->next;
	}
	job->state = JOB_RUNNING;
}

int			hs_bg(const char **args)
{
	t_job		*job;
	const char	*arg;

	if (jc_is_subshell())
		return (1);
	if (!args || !args[0] || args[0][0] != '%' || (args[0] && args[1]))
		return ((ft_dprintf(2, "bg: invalid argument(s): "
			"you must specify a job via single parameter with `%%' prefix\n") & 0) | 1);
	if (jc_get()->active_jobs == NULL)
		return ((ft_dprintf(2, "bg: no active jobs\n") & 0) | 1);
	arg = args[0] + 1;
	job = choose_job(arg);
	if (job == NULL)
		return ((ft_dprintf(2, "bg: no jobs matching criteria: `%%%s'\n", arg) & 0) | 1);
	jc_to_bg(job);
	return (0);

}
