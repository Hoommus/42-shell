/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/11/19 17:33:21 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

struct s_term	*g_term;

int				display_normal_prompt(void)
{
	char	host[1025];
	char	cwd[1025];
	char	*home;
	int		size;

	home = get_env("HOME");
	getcwd(cwd, 1024);
	gethostname(host, 1024);
	if (home && ft_strcmp(cwd, home) == 0 &&
		(cwd[ft_strlen(cwd)] == '/' || cwd[ft_strlen(cwd)] == 0))
		ft_strcpy(cwd, "~");
	host[ft_strchr(host, '.') - host] = 0;
	size = ft_printf(SHELL_PROMPT,
			get_env("USER"), host,
			ft_strrchr(cwd, '/') == NULL ? cwd : ft_strrchr(cwd, '/') + !!(cwd[1] != '\0'),
			g_term->last_cmd_status ? 31 : 32);
	return (size);
}

void			execute_command(char **command)
{
	int			status;

	restore_variables();
	expand_variables(command);
	status = execute(command);
	if (status != 0)
		ft_dprintf(2, "21sh: command not found: %s\n", command[0]);
}

int				shell_loop(void)
{
	int			i;
	int			offset;
	char		**commands;

	while (ponies_teleported())
	{
		display_prompt(g_term->input_state = STATE_NORMAL);
		g_term->last_cmd_status = 0;
		clear_buffer(0);
		commands = read_command();
		i = -1;
		offset = 0;
		while (commands && commands[++i])
			if (ft_strcmp(commands[i], ";") == 0)
			{
				ft_strdel(commands + i);
				execute_command(commands + offset);
				offset = i;
			}
		if (commands && *commands && offset == 0)
			execute_command(commands + offset);
		free_array(commands);
	}
	return (0);
}

/*
** TODO: Add isatty() check and if it is the case, run in non-interactive mode
*/

int				main(int argc, char **argv, char **env)
{
	extern char		**environ;

	g_environ = copy_env(env, environ);
	init_term();
	init_files();
	history_load(g_term->history_file);
	increment_shlvl();
	ft_printf("\n%*s\n%*s\n\n%*s%d (%s)\n",
			g_term->ws_col / 2 + 15, "  Willkommen und bienvenue.  ",
			g_term->ws_col / 2 + 15, "Welcome to 42sh divided by 2.",
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
