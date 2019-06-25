/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/06/25 12:41:42 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "twenty_one_sh.h"
#include "shell_job_control.h"

static void				tstp(int sig)
{
	sig = 0;
}

//void					tstp_alt(int sig, siginfo_t *info, void *ignore)
//{
//	ignore = (void *)(sig & 0);
//	info->si_pid
//}

static void				handle_sigint(int sig)
{
	g_interrupt = sig;
	g_term->last_status = 1;
}

static void				resize(int sig)
{
	struct winsize	size;

	if (sig == SIGWINCH && jc_get()->active_jobs == NULL)
	{
		ft_printf("Fuck, resize!\n");
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
		carpos_update(POS_CURRENT);
	}
}

void					sigchild(int sig, siginfo_t *info, void *ignore)
{
	sig = 0;
	ignore = 0;
	ft_printf("\nFuck, sigchild came from %d with %d signal!\n"
		   "si_value ptr = %p", info->si_pid, info->si_code, info->si_value.sival_ptr);
//	display_prompt(g_term->input_state);
//	buffer_redraw();
}

void					setup_signal_handlers(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(struct sigaction));
	action.__sigaction_u.__sa_handler = &handle_sigint;
	sigaction(SIGINT, &action, NULL);
	action.sa_flags = SA_RESTART;
	action.__sigaction_u.__sa_handler = &tstp;
	sigaction(SIGTSTP, &action, NULL);
	action.sa_flags = SA_SIGINFO | SA_RESTART;
	action.__sigaction_u.__sa_sigaction = &sigchild;
//	sigaction(SIGCHLD, &action, NULL);
	signal(SIGWINCH, &resize);
	signal(SIGPIPE, &tstp);
}
