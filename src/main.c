/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/09/19 14:58:02 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "../include/line_editing.h"

#include <time.h>
struct s_term	*g_term;

int				display_normal_prompt(void)
{
	char	hostname[1024];
	char	*cwd;
	char	*swap;
	char	*home;
	int		size;

	home = get_env("HOME");
	cwd = ft_strnew(1024);
	getcwd(cwd, 1024);
	if (home && ft_strstr(cwd, home) != 0 && cwd[ft_strlen(home)] == '/')
	{
		swap = ft_strsub(cwd, ft_strlen(home) == 0 ? 0 : ft_strlen(home) - 1,
						ft_strlen(cwd) - ft_strlen(home) + 1);
		free(cwd);
		cwd = swap;
		cwd[0] = '~';
	}
	gethostname(hostname, 1024);
	hostname[6] = 0;
	size = ft_printf(SHELL_PROMPT, get_env("USER"), hostname, cwd);
	chfree(cwd);
	return (size);
}

int				shell_loop(void)
{
	int			status;
	int			i;
	char		**commands;
	char		**args;

	while (ponies_teleported())
	{
		display_prompt(g_term->input_state = STATE_NORMAL);
		clear_buffer(0);
		commands = wait_for_input();
		i = 0;
		while (commands && commands[i])
		{
			restore_variables();
			expand_variables(commands + i);
			args = ft_strsplit(commands[i], ' ');
			status = execute(args);
			if (status != 0)
				ft_dprintf(2, "21sh: command not found: %s\n", args[0]);
			free_array(args);
			i++;
		}
		free_array(commands);
	}
	return (0);
}

/*
** TODO: Don't forget to add isatty() and ttyname() usages;
*/
void			init_term(void)
{
	struct winsize	window;
	struct termios	*oldterm;
	struct termios	*newterm;

	oldterm = (struct termios *)malloc(sizeof(struct termios));
	newterm = (struct termios *)malloc(sizeof(struct termios));
	g_term = (struct s_term *)malloc(sizeof(struct s_term));
	g_term->tty_fd = (short)open("/dev/tty", O_RDWR);
	g_term->original_term = oldterm;
	g_term->current_term = newterm;
	tcgetattr(g_term->tty_fd, oldterm);
	ft_memcpy(newterm, oldterm, sizeof(struct termios));
	newterm->c_lflag &= ~(ECHO | ICANON | IEXTEN) | ECHOE | ECHOCTL | ECHONL;
	newterm->c_iflag &= ~(IXOFF);
	tcsetattr(g_term->tty_fd, TCSANOW, newterm);
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	ft_bzero(g_term->buffer, sizeof(char) * (MAX_INPUT + 1));
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	g_term->ws_col = window.ws_col;
	g_term->ws_row = window.ws_row;
	g_term->iterator = 0;
	g_term->input_state = STATE_NORMAL;
	update_caret_position(POS_CURRENT);
}

void			init_files(void)
{
	time_t		rawtime;
	struct tm	*timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	g_term->logfile = (short)open("21sh.log", O_RDWR | O_CREAT | O_TRUNC);
	chmod("21sh.log", 0644);
	ft_dprintf(g_term->logfile, "21sh log [%d]\nDate: %s\n",
				getpid(), asctime(timeinfo));
	g_term->history_file = (short)open(".21sh_history", O_RDWR | O_CREAT);
	chmod(".21sh_history", 0644);
}

int				main(int argc, char **argv, char **env)
{
	extern char		**environ;

	init_term();
	init_files();
	g_environ = copy_env(env, environ);
	increment_shlvl();
	ft_printf("\n%*s\n%*s\n\n%*s%d (%s)\n",
			g_term->ws_col / 2 + 17, "    Willkommen und bienvenue.    ",
			g_term->ws_col / 2 + 17, "  Welcome to 42sh divided by 2.  ",
			5, "Build #", BUILD, BUILD_DATE);
	if (ft_atoi(get_env("SHLVL")) > 2)
		ft_printf("\nRabbit hole depth: %s\n", get_env("SHLVL"));
	if (tgetent(NULL, getenv("TERM")) == ERR)
		ft_printf("\x1b[41;1m%-52s\x1b[0;0m\n\x1b[41;1m%52s\x1b[0;0m\n",
				"Warning: TERM enviroment variable is not set.",
				"Terminal capabilities are somewhat limited.");
	ft_printf("\a");
	setup_signal_handlers();
	shell_loop();
	*argv = argv[argc - argc + 0];
	return (0);
}
