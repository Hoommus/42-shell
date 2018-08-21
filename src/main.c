/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/31 14:45:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/twenty_one_sh.h"

struct s_term	*g_term;

void			display_prompt(void)
{
	char	hostname[1024];
	char	*cwd;
	char	*swap;
	char	*home;

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
	ft_printf(PROMPT, get_env("USER"), hostname, cwd);
	chfree(cwd);
}

int				shell_loop(void)
{
	int			status;
	int			i;
	char		**commands;
	char		**args;

	while (ponies_teleported())
	{
		display_prompt();
		clear_buffer();
		g_term->state = NORMAL;
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

void			init_term(void)
{
	struct winsize	window;
	struct termios	oldterm;
	struct termios	term;
	int				tty_fd;

	tty_fd = open("/dev/tty", O_RDWR);
	ft_printf("tty: %d\n", tty_fd);
	tcgetattr(tty_fd, &oldterm);
	term = oldterm;
	term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	//term.c_lflag &= ECHOCTL | ECHOE | ECHOK | ECHOKE;
	tcsetattr(tty_fd, TCSANOW, &term);
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	g_term = (struct s_term *)malloc(sizeof(struct s_term));
	g_term->term_cols = window.ws_col;
	g_term->term_rows = window.ws_row;
	g_term->iterator = 0;
	ft_bzero(g_term->line_buffer, sizeof(char) * (MAX_INPUT + 1));
	g_term->state = NORMAL;
}

int				main(int argc, char **argv, char **env)
{
	extern char		**environ;

	init_term();
	g_environ = copy_env(env, environ);
	increment_shlvl();
	ft_printf("\n%*s\n%*s\n\n",
			g_term->term_cols / 2 + 17, "    Willkommen und bienvenue.    ",
			g_term->term_cols / 2 + 17, "  Welcome to 42sh divided by 2.  ");
	if (ft_atoi(get_env("SHLVL")) > 2)
		ft_printf("\nRabbit hole (shell) level: %s\n", get_env("SHLVL"));
	if (tgetent(NULL, getenv("TERM")) == ERR)
		printf("\x001b[41;1m%-52s\x001b[0;0m\n\x001b[41;1m%52s\x001b[0;0m\n",
			   "Warning: TERM enviroment variable is not set.",
			   "Terminal capabilities are somewhat limited.");
	setup_signal_handlers();
	hook_listeners();
	shell_loop();
	*argv = argv[argc - argc];
	return (0);
}


