/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_andor_if.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 10:53:11 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/17 19:37:02 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"
#include "shell_script.h"

/*
** TODO: Fix global return status to match zsh's
*/

int		exec_and_if(const t_node *node, bool is_async)
{
	int			status_left;
	int			status_right;
	t_job	*job;
	int			f;

	f = -2;
	if (is_async)
	{
		job = ft_memalloc(sizeof(t_job));
		job->state = JOB_BG;
		f = fork();
	}
	if (f == -2 || f == 0)
	{
		if (f == 0)
			setpgid(getpid(), getpid());
		status_right = 1;
		status_left = exec_node(node->left, NULL, false);
		if (status_left == 0)
			status_right = exec_node(node->right, NULL, false);
		if (f == 0)
			exit(status_right);
		return ((status_right));
	}
	else if (f == -1)
	{
		jc_job_dealloc(&job);
		return ((ft_dprintf(2, SH ": fork error in async AND\n") & 0) | 1);
	}
	setpgid((job->pgid = f), f);
	jc_register_job(job);
	return (0);
}

int		exec_or_if(const t_node *node, bool is_async)
{
	int			status_left;
	int			status_right;
	t_job	*job;
	int			f;

	f = -2;
	if (is_async)
	{
		job = ft_memalloc(sizeof(t_job));
		job->state = JOB_BG;
		f = fork();
	}
	if (f == -2 || f == 0)
	{
		if (f == 0)
			setpgid(getpid(), getpid());
		status_right = 0;
		status_left = exec_node(node->left, NULL, false);
		if (status_left != 0)
			status_right = exec_node(node->right, NULL, false);
		if (f == 0)
			exit(status_right);
		return ((status_right));
	}
	else if (f == -1)
	{
		jc_job_dealloc(&job);
		return ((ft_dprintf(2, SH ": fork error in async OR\n") & 0) | 1);
	}
	setpgid((job->pgid = f), f);
	jc_register_job(job);
	return (0);
}
