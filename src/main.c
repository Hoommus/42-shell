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

char	**wait_for_input(void)
{
	char	*line;
	char	**args;

	while (1)
	{
		if (get_next_line(STDIN_FILENO, &line))
		{
			if (ft_strlen(line) == 0)
				return (NULL);
			args = ft_strsplit(line, ' ');
			free(line);
			break ;
		}
	}
	return (args);
}

void	display_prompt(void)
{
	char	hostname[1024];
	char	*cwd;
	char	*user;
	char	*swap;
	char	*home;

	user = get_env("USER");
	home = get_env("HOME");
	cwd = ft_strnew(1024);
	getcwd(cwd, 1024);
	if (home != NULL && ft_strstr(cwd, home) != 0)
	{
		swap = ft_strsub(cwd, ft_strlen(home) - 1,
						ft_strlen(cwd) - ft_strlen(home) + 1);
		free(cwd);
		cwd = swap;
		cwd[0] = '~';
	}
	 gethostname(hostname, 1024);
	 hostname[6] = 0;
	ft_printf(PROMPT, user, hostname, cwd);
	free(home);
	free(user);
	free(cwd);
}

int		shell_loop(void)
{
	int			status;
	char		**args;

	while (goats_teleported())
	{
		display_prompt();
		args = wait_for_input();
		if (args == NULL)
			continue ;
		status = execute(args);
		if (status != 0)
			ft_dprintf(2, "minishell: command not found: %s\n", args[0]);
		free_array(args);
	}
	return (0);
}

int		main(int argc, char **argv, char **env)
{
	struct winsize	term;
	extern char		**environ;

	*argv = argv[argc - argc];
	ioctl(1, TIOCGWINSZ, &term);
	ft_printf("\n%*s\n%*s\n\n",
			term.ws_col / 2 + 12, "Willkommen und bienvenue.",
			term.ws_col / 2 + 12, " Welcome to Minishell 2. ");
	g_environ = copy_env(env, environ);
	shell_loop();
	return (0);
}
