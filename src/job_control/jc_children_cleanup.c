/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jc_children_cleanup.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:41:56 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:31:54 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_job_control.h"

void					sigpipe_kill_left(t_job *pivot)
{
	int		status;

	pivot = pivot->prev;
	while (pivot)
	{
		if (pivot->pid != 0)
		{
			kill(pivot->pid, SIGPIPE);
			waitpid(pivot->pid, &status, 0);
			pivot->status = WEXITSTATUS(status);
		}
		pivot->state = JOB_TERMINATED;
		pivot = pivot->prev;
	}
}
