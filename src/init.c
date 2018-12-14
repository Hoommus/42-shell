/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:13 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/11 12:27:45 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

#include <time.h>
/*
** TODO: Don't forget to add isatty() and ttyname() usages;
*/

void	init_term(void)
{
	struct winsize	window;
	struct termios	*oldterm;
	struct termios	*newterm;

	oldterm = (struct termios *)ft_memalloc(sizeof(struct termios));
	newterm = (struct termios *)ft_memalloc(sizeof(struct termios));
	g_term = (struct s_term *)ft_memalloc(sizeof(struct s_term));
	g_term->tty_fd = (short)open("/dev/tty", O_RDWR);
	g_term->original_term = oldterm;
	g_term->current_term = newterm;
	tcgetattr(g_term->tty_fd, oldterm);
	ft_memcpy(newterm, oldterm, sizeof(struct termios));
	newterm->c_lflag &= ~(ECHO | ICANON | IEXTEN) | ECHOE | ECHOCTL | ECHONL;
	newterm->c_iflag &= ~(IXOFF);
	TERM_ENFORCE;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	g_term->ws_col = window.ws_col;
	g_term->ws_row = window.ws_row;
	g_term->input_state = STATE_NORMAL;
	init_buffer_vector(MAX_INPUT);
	carpos_update(POS_CURRENT);
}

short	init_fd_at_home(char *filename, int flags)
{
	short	fd;
	char	*home;
	char	*full_path;

	home = get_env("HOME");
	if (home[0] != '\0')
		full_path = ft_strings_join(2, "/", home, filename);
	else
		full_path = ft_strdup(filename);
	if (access(full_path, F_OK) != -1 && access(full_path, 0600) == -1)
		chmod(full_path, 0600);
	fd = (short)open(full_path, O_RDWR | O_CREAT | flags, 0600);
	free(full_path);
	return (fd);
}

void	init_files(void)
{
	time_t		rawtime;
	struct tm	*timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	if (fcntl(1, F_GETFD) == -1)
		dup2(open("/dev/fd/1", O_WRONLY), 1);
	if (fcntl(2, F_GETFD) == -1)
		dup2(open("/dev/fd/2", O_WRONLY), 2);
	g_term->logfile = init_fd_at_home(LOG_FILE, 0);
	g_term->history_file = init_fd_at_home(HISTORY_FILE, 0);
	ft_dprintf(g_term->logfile, "21sh log [pid %d]\nDate: %s\n", getpid(),
				asctime(timeinfo));
}
