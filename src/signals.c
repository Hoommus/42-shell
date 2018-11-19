/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:46:06 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/19 15:35:13 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "../include/line_editing.h"

void	tstp(int sig)
{
	//sig = 0;
	clear_buffer(0);
	ft_printf("Received SIGTSTP (%d)\n", sig);
	display_normal_prompt();
	tcsetattr(g_term->tty_fd, TCSANOW, g_term->current_term);
	g_term->input_state = STATE_NORMAL;
	ft_dprintf(0, "\4");
	ft_printf("\7");
}

void	ignore(int sig)
{
	sig = 0;
	if (g_term->running_process == 0)
	{
		clear_buffer(0);
		ft_printf("\n");
		display_normal_prompt();
		g_term->input_state = STATE_NORMAL;
	}
}

void	resize(int sig)
{
	struct winsize	size;

	if (sig == SIGWINCH)
	{
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		g_term->ws_row = size.ws_row;
		g_term->ws_col = size.ws_col;
		update_caret_position(POS_CURRENT);
//		caret_move(ft_utf_strlen(g_term->buffer), D_LEFT);
//		buffer_redraw(0);
	}
}

void	fatal(int sig)
{
	sig = 0;
	tcsetattr(g_term->tty_fd, TCSANOW, g_term->original_term);
}

void	setup_signal_handlers(void)
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
