/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_child_blocker.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/04 11:56:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/05 16:19:03 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_job_control.h"

static sigset_t	g_old;

const sigset_t	*sigchild_block(void)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &mask, &g_old);
	return (&g_old);
}

const sigset_t	*sigchild_unblock(void)
{
	sigset_t	mask;

	sigemptyset(&mask);
//	sigaddset(&mask, SIGCHLD);
	sigprocmask(SIG_SETMASK, &mask, &g_old);
	return (&g_old);
}

bool			sigchild_is_blocked(void)
{
	sigset_t	mask;

	sigprocmask(SIG_SETMASK, NULL, &mask);
	return (sigismember(&mask, SIGCHLD));
}
