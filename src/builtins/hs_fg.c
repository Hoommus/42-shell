
#include "shell_job_control.h"
#include "twenty_one_sh.h"

t_job		*search_job_id(int id)
{
	t_job		*list;
	t_job		*job;

	job = NULL;
	list = jc_get()->active_jobs;
	while (list && !job)
	{
		if (list->id == id)
			job = list;
		list = list->next;
	}
	return (job);
}

t_job		*search_string(const char *str, bool anywhere)
{
	str = str + anywhere;
	return (NULL);
}

void		jc_to_fg(t_job *job)
{
	t_proc	*segments;

	job->state = JOB_CONTINUED;
	jc_format_job(job);
	segments = job->procs;
	while (segments)
	{
		if (segments->is_stopped && !segments->is_completed)
		{
			TERM_APPLY_CONFIG(segments->context->term_config);
			kill(segments->pid, SIGCONT);
			segments->is_stopped = false;
		}
		segments = segments->next;
	}
	job->state = JOB_RUNNING;
	tcsetpgrp(0, job->pgid);
	jc_wait(job);
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(0, g_term->shell_pgid);
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	signal(SIGTTOU, SIG_DFL);
}

int			hs_fg(const char **args)
{
	t_job		*list;
	t_job		*job;
	const char	*arg;

	list = jc_get()->active_jobs;
	if (!args || !args[0] || args[0][0] != '%' || (args[0] && args[1]))
		return ((ft_dprintf(2, "fg: invalid argument(s): "
			"you must specify a job via single parameter starting with `%%'\n") & 0) | 1);
	arg = args[0] + 1;
	if (arg[1] == 0 && (arg[0] == '%' || arg[0] == '+'))
	{
		while (list && list->next)
			list = list->next;
		job = list;
	}
	else if (arg[1] == 0 && (arg[0] == '-'))
	{
		while (list && list->next && list->next->next)
			list = list->next;
		job = list;
	}
	else if (is_string_numeric(arg, 10))
		job = search_job_id(ft_atoi(arg));
	else
		job = search_string(arg + (arg[0] == '?'), arg[0] == '?');
	if (job == NULL)
		return ((ft_dprintf(2, "fg: no jobs matching criteria: `%s'\n", arg - 1) & 0) | 1);
	jc_to_fg(job);
	return (0);
}
