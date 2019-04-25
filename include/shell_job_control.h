/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_job_control.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:40:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:31:54 by vtarasiu         ###   ########.fr       */
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
	int						exit_status;
	const struct s_command	*cmd;
	t_context				*context;
	struct s_job			*prev;
	struct s_job			*next;
}						t_job;

struct					s_job_control
{
	t_job			*jobs;
	t_job			*job_queue;
	t_context		*shell_context;
	pid_t			shell_pid;
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
void					jc_collect_zombies(void);

/*
** Auxiliary
*/

int						forknrun(t_job *job, char *path);
void					close_redundant_fds(t_context *context);
void					sigpipe_kill_left(t_job *pivot);

#endif
