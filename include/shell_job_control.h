/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_job_control.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:40:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/27 18:43:28 by vtarasiu         ###   ########.fr       */
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
	JOB_RUNNING = 0,
	JOB_BG,
	JOB_STOPPED,
	JOB_LAUNCHED,
	JOB_EXITED,
	JOB_TERMINATED = 6,
	JOB_CONTINUED,

	JOB_SIGHUP = 31,
	JOB_SIGINT,
	JOB_SIGQUIT,
	JOB_SIGILL,
	JOB_SIGTRAP,
	JOB_SIGABRT,
	JOB_SIGEMT,
	JOB_SIGFPE,
	JOB_SIGKILL,
	JOB_SIGBUS,
	JOB_SIGSEGV,
	JOB_SIGSYS,
	JOB_SIGPIPE,
	JOB_SIGALRM,
	JOB_SIGTERM,
	JOB_SIGURG,
	JOB_SIGSTOP,
	JOB_SIGTSTP,
	JOB_SIGCONT,
	JOB_SIGCHLD,
	JOB_SIGTTIN,
	JOB_SIGTTOU,
	JOB_SIGIO,
	JOB_SIGXCPU,
	JOB_SIGXFSZ,
	JOB_SIGVTALRM,
	JOB_SIGPROF,
	JOB_SIGWINCH,
	JOB_SIGINFO,
	JOB_SIGUSR1,
	JOB_SIGUSR2
};


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
}						t_proc;

typedef struct			s_job_alt
{
	char				*command;
	t_proc				*procs;
	pid_t				pgid;
	bool				notified;
	int					id;
	enum e_job_state	state;
	struct s_job_alt	*next;
}						t_job;

struct					s_job_control
{
	t_context		*shell_context;
	pid_t			shell_pid;
	t_job			*active_jobs;
	t_job			*tmp_job;
};

void					jc_init(t_context *context);
struct s_job_control	*jc_get(void);
void					jc_register_job(t_job *job);
void					jc_unregister_job(pid_t pgid);
int						jc_wait(t_job *job);
t_proc					*process_create(t_command *command, t_context *context);
t_proc					*process_list_add(t_proc **pipeline, t_proc *segment);
void					process_list_destroy(t_proc **pipeline);

int						jc_launch(t_job *job, bool is_async);

t_job					*jc_tmp_add(t_proc *segment);
int						jc_tmp_finalize(bool is_async);
void					jc_job_dealloc(t_job **job);

enum e_job_state		poll_pipeline(t_job *job, bool wnohang);


/*
** Auxiliary
*/
void					handle_signaled(t_job *job, int status);
char					*jc_state_str(enum e_job_state state);
void jc_format_job(const t_job *job);
int						forknrun(t_job *job, t_proc *process, char *path, bool is_async);
void					close_redundant_fds(t_context *context);

#endif
