/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_job_control.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:40:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/15 14:12:51 by vtarasiu         ###   ########.fr       */
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

typedef struct			s_job
{
	pid_t					pid;
	enum e_job_state		state;
	int						status;
	int						wexitstatus;
	const struct s_command	*cmd;
	t_context				*context;
	struct s_job			*prev;
	struct s_job			*next;
}						t_job;

/*
** needs_wait field indicates if you have to wait for this segment to finish.
** It exists to make pipelines inside subshells and subshells inside pipelines possible.
*/

typedef struct			s_segment
{
	int					status;
	pid_t				pid;
	bool				needs_wait : 1;
	bool				is_completed : 1;
	bool				is_stopped : 1;
	t_context			*context;
	struct s_command	*command;
	struct s_segment	*next;
}						t_pipe_segment;

typedef struct			s_job_alt
{
	char				*command;              /* command line, used for messages */
	t_pipe_segment		*first_process;     /* list of processes in this job */
	t_pipe_segment		*pipeline;
	pid_t pgid;                 /* process group ID */
	char notified;              /* true if user told about stopped job */
	struct termios tmodes;      /* saved terminal modes */
	enum e_job_state	state;
	struct job			*next;           /* next active job */
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
	t_job			*jobs;
	t_job			*job_queue;
	t_context		*shell_context;
	pid_t			shell_pid;
	int				queue_size;
};

void					jc_init(t_context *context);
struct s_job_control	*jc_get(void);
void					jc_register_job(t_job *job);
void					jc_unregister_job(pid_t id);
t_job					*jc_create_job(const struct s_command *cmd,
	t_context *context, enum e_job_state job_class);

void					jc_enqueue_job(t_job *job);
t_job					*jc_dequeue_job(pid_t pid, t_job *job);
int						jc_execute_pipeline_queue(void);
void					jc_destroy_queue(void);

t_pipe_segment			*pipe_segment_new(t_command *command, t_context *context, bool is_subshell);
t_pipe_segment			*pipeline_add_segment(t_pipe_segment **pipeline, t_pipe_segment *segment);
void					pipeline_destroy(t_pipe_segment **pipeline);
int						pipeline_execute(t_pipe_segment *pipeline);
int						pipeline_execute_segment(t_pipe_segment *segment);

/*
** Auxiliary
*/

int						forknrun(t_job *job, char *path);
void					close_redundant_fds(t_context *context);

#endif
