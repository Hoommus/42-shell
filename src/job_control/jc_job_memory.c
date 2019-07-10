/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_job_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 14:42:34 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 14:42:34 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

void			jc_job_dealloc(t_job **job)
{
	process_list_destroy(&(*job)->procs);
	free((*job)->command);
	free(*job);
	*job = NULL;
}

t_job			*jc_tmp_add(t_proc *segment)
{
	struct s_job_control	*jc;

	jc = jc_get();
	if (jc->tmp_job == NULL)
		jc->tmp_job = ft_memalloc(sizeof(t_job));
	process_list_add(&jc->tmp_job->procs, segment);
	return (jc->tmp_job);
}

int				jc_tmp_finalize(bool is_async)
{
	t_job		*job;
	int			status;

	job = jc_get()->tmp_job;
	if (job == NULL)
		return (0);
	jc_get()->tmp_job = NULL;
	status = jc_launch(job, is_async);
	return (status);
}
