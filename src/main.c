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

ssize_t	ponies_teleported(void)
{
	ssize_t			ponies;
	static int		fd;

	if (fd == 0)
		fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		read(fd, &ponies, sizeof(ssize_t));
		if (ponies == 0)
			ponies += 1348;
		return (ABS(ponies));
	}
}

void	display_prompt(void)
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

int		shell_loop(void)
{
	int			status;
	int			i;
	char		**commands;
	char		**args;

	while (ponies_teleported())
	{
		display_prompt();
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

int		_ma_in_(int argc, char **argv, char **env)
{
	struct winsize	window;
	extern char		**environ;
	int				tty_fd;
	struct termios	term;

	*argv = argv[argc - argc];
	tty_fd = open("/dev/tty", O_RDWR);
	tcgetattr(tty_fd, &term);
	term.c_lflag &= ~(ICANON);
	tcsetattr(tty_fd, TCSANOW, &term);
	ioctl(1, TIOCGWINSZ, &window);
	ft_printf("\n%*s\n%*s\n\n",
			window.ws_col / 2 + 17, "    Willkommen und bienvenue.    ",
			window.ws_col / 2 + 17, "  Welcome to 42sh divided by 2.  ");
	g_environ = copy_env(env, environ);
	increment_shlvl();
	setup_signal_handlers();
	shell_loop();
	return (0);
}


