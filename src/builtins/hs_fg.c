
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

int			jc_to_fg(t_job *job)
{
	t_proc	*segments;
	int		status;

	job->state = JOB_CONTINUED;
	jc_format_job(job);
	job->state = JOB_RUNNING;
	segments = job->procs;
	TERM_APPLY_CONFIG(segments->context->term_config);
	tcsetpgrp(0, job->pgid);
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
	status = jc_wait(job);
	tcsetpgrp(0, g_term->shell_pgid);
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	return (status);
}

t_job		*choose_job(const char *criteria)
{
	t_job		*list;

	list = jc_get()->active_jobs;
	if (criteria[1] == 0 && (criteria[0] == '%' || criteria[0] == '+'))
	{
		while (list && list->next)
			list = list->next;
		return (list);
	}
	else if (criteria[1] == 0 && (criteria[0] == '-'))
	{
		while (list && list->next && list->next->next)
			list = list->next;
		return (list);
	}
	else if (is_string_numeric(criteria, 10))
		return (search_job_id(ft_atoi(criteria)));
	return (search_string(criteria + (criteria[0] == '?'), criteria[0] == '?'));
}

int			hs_fg(const char **args)
{
	t_job		*job;
	const char	*arg;

	if (jc_is_subshell())
		return (1);
	if (!args || !args[0] || args[0][0] != '%' || (args[0] && args[1]))
		return ((ft_dprintf(2, "fg: invalid argument(s): "
			"you must specify a job via single parameter starting with `%%'\n") & 0) | 1);
	if (jc_get()->active_jobs == NULL)
		return ((ft_dprintf(2, "fg: no active jobs\n") & 0) | 1);
	arg = args[0] + 1;
	job = choose_job(arg);
	if (job == NULL)
		return ((ft_dprintf(2, "fg: no jobs matching criteria: `%%%s'\n", arg) & 0) | 1);
	return (jc_to_fg(job));
}
