#include "../include/minishell.h"

ssize_t	goats_teleported(void)
{
	ssize_t	goats;
	int		fd;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (1);
	else
	{
		read(fd, &goats, sizeof(ssize_t));
		close(fd);
		if (goats == 0)
			goats += 1348;
		return (ABS(goats));
	}
}

char	**wait_for_input()
{
	char	*line;
	char	**args;

	while (1)
	{
		if (get_next_line(STDIN_FILENO, &line))
		{
			if (ft_strlen(line) == 0)
				return (NULL);
			args = ft_strsplit_wht(line);
			free(line);
			break ;
		}
	}
	return (args);
}

void	display_prompt(char **environ)
{
	char	hostname[1024];
	char	*pwd;
	char	*user;
	char	*swap;
	char	*home;

	user = get_env("USER", environ);
	home = get_env("HOME", environ);
	pwd = get_env("PWD", environ);
	if (ft_strstr(pwd, home) != 0)
	{
		swap = ft_strsub(pwd, ft_strlen(home) - 1,
						ft_strlen(pwd) - ft_strlen(home) + 1);
		free(pwd);
		pwd = swap;
		pwd[0] = '~';
	}
	 gethostname(hostname, 1024);
	 hostname[6] = 0;
	ft_printf(PROMPT, user, hostname, pwd);
	free(home);
	free(user);
	free(pwd);
}

int		shell_loop(void)
{
	int			status;
	extern char	**environ;
	char		**args;

	while (goats_teleported())
	{
		display_prompt(environ);
		args = wait_for_input();
		if (args == NULL)
			continue ;
		status = execute(args, environ);

		if (status != 0)
			ft_printf("minishell: command not found: %s\n", args[0]);
	}
	return (0);
}

int		main(int argc, char **argv, char **env)
{
	struct winsize	term;

	argv[argc - 1] = env[0];
	ioctl(1, TIOCGWINSZ, &term);
	ft_printf("%*s\n%*s\n%*s\n%*s\n",
			term.ws_col / 2 + 14, "/***************************\\",
			term.ws_col / 2 + 14, "| Willkommen und bienvenue. |",
			term.ws_col / 2 + 14, "|  Welcome to Minishell 2.  |",
			term.ws_col / 2 + 14, "\\***************************/");
	shell_loop();
	return (0);
}
