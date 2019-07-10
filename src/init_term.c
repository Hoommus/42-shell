/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 14:52:49 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 14:54:23 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

#define LFLAGS (~(ECHO | ECHOK | ECHONL | ECHOCTL | ICANON) | ISIG | IEXTEN)

static void		init_termios(struct termios *newterm)
{
	struct winsize	window;

	newterm->c_lflag &= LFLAGS;
	newterm->c_iflag &= ~(IXOFF) | BRKINT;
	newterm->c_cflag &= ~(CREAD);
	newterm->c_cc[VMIN] = 1;
	newterm->c_cc[VTIME] = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	g_term->ws_col = window.ws_col;
	g_term->ws_row = window.ws_row;
}

struct termios	*init_term(void)
{
	struct termios	*oldterm;
	struct termios	*newterm;

	oldterm = (struct termios *)ft_memalloc(sizeof(struct termios));
	newterm = (struct termios *)ft_memalloc(sizeof(struct termios));
	g_term->tty_fd = (short)open_wrapper("/dev/tty", O_RDWR);
	if (!isatty(STDIN_FILENO) || g_term->tty_fd == -1)
		g_term->input_state = STATE_NON_INTERACTIVE;
	else
	{
		g_term->input_state = STATE_NORMAL;
		g_term->context_original->term_config = oldterm;
		tcgetattr(g_term->tty_fd, oldterm);
		ft_memcpy(newterm, oldterm, sizeof(struct termios));
		init_termios(newterm);
		close_wrapper(0);
		close_wrapper(1);
		close_wrapper(2);
		dup2(g_term->tty_fd, 0);
		dup2(g_term->tty_fd, 1);
		dup2(g_term->tty_fd, 2);
		close_wrapper(g_term->tty_fd);
	}
	g_term->fallback_input_state = g_term->input_state;
	return (newterm);
}
