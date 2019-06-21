/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_job_control.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:40:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/20 16:19:58 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_JOB_CONTROL_H
# define SHELL_JOB_CONTROL_H

# include "libft.h"
# include "twenty_one_sh.h"
# include "shell_script.h"
# include <stdbool.h>
# include <sys/types.h>

/*
** Job Control to Major Tom
** Commencing countdown, queue is on
** Check execution and may God's love be with you
*/

enum					e_job_state
{
	JOB_FG = 0,
	JOB_BG,
	JOB_PIPE,
	JOB_STOPPED,
	JOB_SIGTTXX,
	JOB_SIGTTOU,
	JOB_EXITED,
	JOB_TERMINATED
};

extern const char		*g_state_names[];

/*
** needs_wait field indicates if you have to wait for this segment to finish.
** It exists to make pipelines inside subshells and subshells inside pipelines possible.
*/

typedef struct			s_segment
{
	int					status;
	pid_t				pid;
	bool				is_completed : 1;
	bool				is_stopped : 1;
	t_context			*context;
	struct s_command	*command;
	struct s_segment	*next;
}						t_pipe_segment;

typedef struct			s_job_alt
{
	char				*command;
	t_pipe_segment		*pipeline;
	pid_t				pgid;
	bool				notified;
	int					id;
	enum e_job_state	state;
	struct s_job_alt	*next;
}						t_job_alt;

typedef struct			s_process
{
	pid_t				pid;
	int					wexitstatus;
	struct s_command	*command;
	struct s_process	*next;
}						t_process;

struct					s_job_control
{
	t_context		*shell_context;
	pid_t			shell_pid;
	t_job_alt		*active_jobs;
	t_job_alt		*tmp_job;
};

void					jc_init(t_context *context);
struct s_job_control	*jc_get(void);
void					jc_register_job(t_job_alt *job);
void					jc_unregister_job(pid_t id);

t_pipe_segment			*pipe_segment_new(t_command *command, t_context *context, bool is_subshell);
t_pipe_segment			*pipeline_add_segment(t_pipe_segment **pipeline, t_pipe_segment *segment);
void					pipeline_destroy(t_pipe_segment **pipeline);
int						pipeline_execute(t_pipe_segment *pipeline);
int						pipeline_execute_segment(t_pipe_segment *segment);

int						launch_job(t_job_alt *job, bool is_async);

t_job_alt				*jc_tmp_add(t_pipe_segment *segment);
int						jc_tmp_finalize(bool is_async);
void					jc_job_dealloc(t_job_alt **job);
/*
** Auxiliary
*/

int						forknrun(t_job_alt *job, t_pipe_segment *process, char *path, bool is_async);
void					close_redundant_fds(t_context *context);

#endif
