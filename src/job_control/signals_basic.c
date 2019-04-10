/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/22 17:12:27 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_history.h"
#include "line_editing.h"

static void				tstp(int sig)
{
	TERM_APPLY_CONFIG(g_term->context_current->term_config);
	buff_clear(0);
	ft_printf("Received SIGTSTP (%d)\n", sig);
	write(0, "\4", 1);
}

void					ignore(int sig)
{
	sig = 0;
}

void					handle_sigint(int sig)
{
	extern volatile sig_atomic_t	g_is_interrupted;

	g_is_interrupted = sig;
}

static void				resize(int sig)
{
	struct winsize	size;

	if (sig == SIGWINCH)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
		carpos_update(POS_CURRENT);
	}
}

void					fatal(int sig)
{
	sig = 0;
}

void					setup_signal_handlers(void)
{
	struct sigaction	action;

 	ft_bzero(&action, sizeof(struct sigaction));
	action.__sigaction_u.__sa_handler = &tstp;
	sigaction(SIGTSTP, &action, NULL);
	action.__sigaction_u.__sa_handler = &handle_sigint;
	sigaction(SIGINT, &action, NULL);
	signal(SIGWINCH, &resize);
}
