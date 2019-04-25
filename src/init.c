/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:13 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:31:54 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_environ.h"
#include "line_editing.h"

#include <time.h>
void			init_shell_context(void)
{
	extern const char	**environ;

	g_term = (struct s_term *)ft_memalloc(sizeof(struct s_term));
	g_term->context_original = ft_memalloc(sizeof(t_context));
	g_term->context_original->environ =
		environ_create_vector(VARIABLES_VECTOR_INITIAL_SIZE);
	if (fcntl(STDIN_FILENO, F_GETFD) != -1)
		context_add_fd(g_term->context_original, 0, 0, "stdin");
	if (fcntl(STDOUT_FILENO, F_GETFD) != -1)
		context_add_fd(g_term->context_original, 1, 1, "stdout");
	if (fcntl(STDERR_FILENO, F_GETFD) != -1)
		context_add_fd(g_term->context_original, 2, 2, "stderr");
	environ_from_array(g_term->context_original->environ, environ);
	g_term->context_current = context_duplicate(g_term->context_original, true);
	ft_printf("Deallocating vector...\n");
	environ_deallocate_vector(g_term->context_current->environ);
	ft_printf("Deallocated!\n");
	free(g_term->context_current->term_config);
	g_term->context_current->environ = g_term->context_original->environ;
	g_term->context_current->term_config = init_term();
	ft_printf("Switching context...\n");
	context_switch(g_term->context_current);
}

struct termios	*init_term(void)
{
	struct winsize	window;
	struct termios	*oldterm;
	struct termios	*newterm;

	oldterm = (struct termios *)ft_memalloc(sizeof(struct termios));
	newterm = (struct termios *)ft_memalloc(sizeof(struct termios));
	g_term->tty_fd = (short)open_wrapper("/dev/tty", O_RDWR);
	if (!isatty(STDIN_FILENO) || g_term->tty_fd == -1)
		g_term->input_state = STATE_NON_INTERACTIVE;
	else
		g_term->input_state = STATE_NORMAL;
	g_term->context_original->term_config = oldterm;
	tcgetattr(g_term->tty_fd, oldterm);
	ft_memcpy(newterm, oldterm, sizeof(struct termios));
	newterm->c_lflag &= ~(ECHO | ICANON | IEXTEN) | ECHOE | ECHONL;
	newterm->c_iflag &= ~(IXOFF);
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	g_term->ws_col = window.ws_col;
	g_term->ws_row = window.ws_row;
	init_buffer_vector(MAX_INPUT);
	return (newterm);
}

short			init_fd_at_home(char *filename, int flags)
{
	t_var	*var;
	short	fd;
	char	*full_path;

	var = get_env_v(NULL, "HOME");
	if (var && var->value && var->value[0] != '\0')
		full_path = ft_strings_join(2, "/", var->value, filename);
	else
		full_path = ft_strdup(filename);
	if (access(full_path, F_OK) != -1 && access(full_path, 0600) == -1)
		chmod(full_path, 0600);
	fd = (short)openm_wrapper(full_path, O_RDWR | O_CREAT | flags, 0600);
	free(full_path);
	return (fd);
}

void			init_files(void)
{
	time_t		rawtime;
	struct tm	*timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	g_term->logfile = init_fd_at_home(LOG_FILE, 0);
	g_term->history_file = init_fd_at_home(HISTORY_FILE, 0);
	ft_dprintf(g_term->logfile, "21sh log [pid %d]\nDate: %s\n", getpid(),
				asctime(timeinfo));
}

void			parse_args(int argc, char **argv)
{
	environ_push_entry(g_term->context_original->environ, "0",
		argv[0], SCOPE_SHELL_LOCAL);
	argc = 0;
}
