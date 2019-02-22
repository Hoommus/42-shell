/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/18 13:34:03 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_history.h"
#include "line_editing.h"

static void		tstp(int sig)
{
	//sig = 0;
	TERM_ENFORCE;
	buff_clear(0);
	ft_printf("Received SIGTSTP (%d)\n", sig);
	display_normal_prompt();
	g_term->input_state = STATE_NORMAL;
	ft_printf("\7");
	write(0, "\4", 1);
}

static void		ignore(int sig)
{
	extern t_history	*g_history;

	sig = 0;
	caret_move(g_term->buffer->size - g_term->buffer->iterator, D_RIGHT);
	buff_clear(0);
	ft_printf("\n");
	g_term->input_state = STATE_NORMAL;
	if (g_term->running_process == 0)
		display_normal_prompt();
}

static void		resize(int sig)
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

void			fatal(int sig)
{
	sig = 0;
	TERM_RESTORE;
}

void			setup_signal_handlers(void)
{
	struct sigaction	action;

	ft_bzero(&action, sizeof(struct sigaction));
	action.sa_flags = SA_RESTART;
	action.__sigaction_u.__sa_handler = &tstp;
	sigaction(SIGTSTP, &action, NULL);
//	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, &ignore);
	//signal(SIGSEGV, &fatal);
	//signal(SIGQUIT, &fatal);
	//signal(SIGTSTP, &tstp);
	signal(SIGWINCH, &resize);
}
