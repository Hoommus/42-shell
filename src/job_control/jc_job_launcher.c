
#include <errno.h>
#include "shell_job_control.h"
#include "shell_builtins.h"

extern bool				g_is_subshell_env;
extern bool				g_is_forked_shell;

void				jc_job_dealloc(t_job **job)
{
	pipeline_destroy(&(*job)->pipeline);
	free((*job)->command);
	free(*job);
	*job = NULL;
}

t_job			*jc_tmp_add(t_pipe_segment *segment)
{
	struct s_job_control	*jc = jc_get();

	if (jc->tmp_job == NULL)
		jc->tmp_job = ft_memalloc(sizeof(t_job));
	pipeline_add_segment(&jc->tmp_job->pipeline, segment);
	return (jc->tmp_job);
}


void					close_redundant_fds(t_context *context)
{
	struct s_fd_lst		*list;

	if (context == NULL)
		return ;
	list = context->fd_list;
	while (list)
	{
		if (ft_strcmp(list->label, "rdr_duped_1337"))
		{
			if (list->current > 2)
				close(list->current);
		}
		list = list->next;
	}
}

static char				*path_to_target(t_pipe_segment *segment)
{
	const char	**args = (const char **)segment->command->args;
	t_var		*var;
	int			i;
	char		*swap;
	char		**paths;

	if (ft_strchr(args[0], '/') != NULL)
		return (ft_strdup(args[0]));
	var = get_env_v(NULL, "PATH");
	if (!var || !var->value)
		return (NULL);
	paths = ft_strsplit(var->value, ':');
	i = 0;
	swap = NULL;
	while (paths[i])
	{
		swap = ft_strings_join(2, "/", paths[i], (char *)args[0], NULL);
		if (access(swap, F_OK) == 0)
			break ;
		ft_memdel((void **)&swap);
		i++;
	}
	free_array((void **)paths);
	return (swap);
}

static int				forknrun_builtin(t_pipe_segment *segment, int (*bltin)(const char **))
{
	int						status;

	if ((status = fork()) == 0)
	{
		context_switch(segment->context);
		exit(bltin((const char **)segment->command->args + 1));
	}
	else if (status == -1)
		ft_dprintf(2, SH ": fork for builtin %s failed\n", segment->command->args[0]);
	else
		context_deep_free(&segment->context);
	return (0);
}

static int				run_builtin(t_pipe_segment *segment, bool is_async)
{
	extern struct s_builtin	g_builtins[];
	const char				*bltin = segment->command->args ? segment->command->args[0] : NULL;
	int						i;

	i = -1;
	while (bltin && g_builtins[++i].name)
		if (ft_strcmp(bltin, g_builtins[i].name) == 0)
		{
			if (!is_async)
			{
				context_switch(segment->context);
				segment->status = g_builtins[i].
					function((const char **)segment->command->args + 1);
				context_switch(jc_get()->shell_context);
				return (segment->status);
			}
			else
				return (forknrun_builtin(segment, g_builtins[i].function));
		}
	return (-512);
}

#define DIRTY_HACK(err) (ft_dprintf(2, err, process->command->args[0]), -1024)

static int				run_regular(t_job *job, const t_pipe_segment *process, bool is_async)
{
	int				status;
	char			*bin;

	status = -256;
	if ((bin = path_to_target((t_pipe_segment *)process)) != NULL
		&& access(bin, F_OK) == -1 && ft_strchr(bin, '/') != NULL)
		ft_dprintf(2, ERR_NO_SUCH_FILE, process->command->args[0]);
	else if (bin != NULL && is_dir(bin))
		ft_dprintf(2, ERR_IS_A_DIRECTORY, process->command->args[0]);
	else if (bin != NULL && access(bin, X_OK) == -1)
		ft_dprintf(2, ERR_PERMISSION_DENIED, process->command->args[0]);
	else if (bin == NULL)
		ft_dprintf(2, ERR_COMMAND_NOT_FOUND, process->command->args[0]);
	else if (!g_interrupt)
		status = forknrun(job, (t_pipe_segment *) process, bin, is_async);
	ft_memdel((void **)&bin);
	return (status);
}

int						execute_segments(t_job *job, bool is_async)
{
	const t_pipe_segment	*list = job->pipeline;
	int						s;

	if ((list && list->next) || is_async)
		g_is_subshell_env = true;
	while (list)
	{
		if ((s = run_builtin((t_pipe_segment *)list, is_async)) != -512 && !list->next)
			return (s);
		else if (s == -512)
			run_regular(job, list, is_async);
		close_redundant_fds(list->context);
		list = list->next;
	}
	g_is_subshell_env = false;
	return (-1024);
}

/*
 *
 *
 */

int					wait_for_job(t_job *job)
{
	t_pipe_segment		*procs;
	int					status;
	enum e_job_state	state;
	int					w;

	state = JOB_RUNNING;
	status = 0;
	procs = job->pipeline;
	ft_printf("Now waiting for job\n");
	while (procs)
	{
		if ((w = waitpid(procs->pid, &status, WUNTRACED)) == procs->pid)
		{
			procs->status = status;
			if (WIFSTOPPED(status) || WIFSIGNALED(status))
			{
				state = WIFSTOPPED(status) ? JOB_STOPPED : JOB_SIGTTXX;
				status = -1;
				break ;
			}
			state = WEXITSTATUS(status);
		}
		procs = procs->next;
	}
	if (state != JOB_STOPPED)
		state = JOB_TERMINATED;
	job->state = state;
	ft_printf("Finished waiting\n");
	return (status);
}

int					launch_job(t_job *job, bool is_async)
{
	t_job			*list;
	int				status;
	int				i;

	g_term->input_state = STATE_JOB_IN_FG;
	job->command = ft_strarr_join(" ", job->pipeline->command->args);
	execute_segments(job, is_async);
	status = 0;
	signal(SIGTTOU, SIG_IGN);
	if (g_is_subshell_env || !is_async)
		status = wait_for_job(job);
//	tcdrain(0);
//	tcdrain(1);
//	tcdrain(2);
	if (tcsetattr(0, TCSAFLUSH, g_term->context_current->term_config) == -1)
		ft_dprintf(2, SH ": tcsetattr error: %s\n", strerror(errno));
	if (tcsetpgrp(0, g_term->shell_pgid) == -1)
		ft_dprintf(2, SH ": tcsetpgrp error: %s\n", strerror(errno));
	signal(SIGTTOU, SIG_DFL);
	if (!g_is_subshell_env && (is_async || status == -1))
	{
		i = 1;
		if (!(list = jc_get()->active_jobs))
			jc_get()->active_jobs = job;
		else
		{
			while (list->next && ++i)
				list = list->next;
			((t_job *)list)->next = job;
		}
		job->id = i;
		//jc_format_job(job);
	}
	else
		jc_job_dealloc(&(job));
	return (status);
}

int					jc_tmp_finalize(bool is_async)
{
	t_job		*job;
	int			status;

	job = jc_get()->tmp_job;
	jc_get()->tmp_job = NULL;
	status = launch_job(job, is_async);
	if (!is_async)
		;
	return (status);
}
