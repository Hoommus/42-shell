/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/16 15:05:44 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_history.h"

struct s_term		*g_term;

int					display_normal_prompt(void)
{
	t_var	*home;
	t_var	*user;
	char	host[1025];
	char	cwd[1025];
	int		size;

	home = get_env_v(NULL, "HOME");
	getcwd(cwd, 1024);
	gethostname(host, 1024);
	if (home && home->value && ft_strcmp(cwd, home->value) == 0 &&
		(cwd[ft_strlen(cwd)] == '/' || cwd[ft_strlen(cwd)] == 0))
		ft_strcpy(cwd, "~");
	host[ft_strchr(host, '.') - host] = 0;
	user = get_env_v(NULL, "USER");
	size = ft_printf(SHELL_PROMPT,
		user ? user->value : "%username%", host,
		ft_strrchr(cwd, '/') == NULL ? cwd
									: ft_strrchr(cwd, '/') + !!(cwd[1] != '\0'),
		g_term->last_cmd_status ? 31 : 32);
	return (size);
}

void				execute_command(char **command)
{
	int			status;

	expand_variables(command);
	// TODO: run_script(tokenizer(), false);
	status = execute(command);
	if (status != 0)
		ft_dprintf(2, "21sh: command not found: %s\n", command[0]);
}

int					shell_loop(void)
{
	int			i;
	int			offset;
	char		**commands;

	while (ponies_teleported())
	{
		display_prompt(g_term->input_state = STATE_NORMAL);
		g_term->last_cmd_status = 0;
		buff_clear(0);
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
		free_array((void **)commands);
	}
	return (0);
}

extern const char	*__asan_default_options(void);

extern const char	*__asan_default_options(void)
{
	return ("help='0'"
			"handle_segv='1'"
			"handle_abort='1'"
			"handle_sigill='1'"
			"handle_sigfpe='1'"
			"allow_user_segv_handler='1'");
}

void				print_messages(void)
{
	t_var	*var;

	ft_printf("\n%*s\n%*s\n\n%*s%d (%s)\n",
			  g_term->ws_col / 2 + 15, "  Willkommen und bienvenue.  ",
			  g_term->ws_col / 2 + 15, "Welcome to 42sh divided by 2.",
			  5, "Build #", BUILD, BUILD_DATE);
	var = get_env_v(NULL, "SHLVL");
	if (var && ft_atoi(var->value) > 2)
		ft_printf("\nRabbit hole depth: %s\n", var->value);
	var = get_env_v(NULL, "TERM");
	if (!var || tgetent(NULL, var->value) == ERR)
		ft_printf("\x1b[41;1m%-52s\x1b[0;0m\n\x1b[41;1m%52s\x1b[0;0m\n",
				  "Warning: TERM enviroment variable is not set.",
				  "Terminal capabilities are somewhat limited.");

}

int					main(int argc, char **argv)
{
	extern char		**environ;

	init_shell_context();
	g_term->context_current = context_duplicate(g_term->context_original, true);
	ft_memdel((void **)&(g_term->context_current->term_config));
	g_term->context_current->term_config = init_term();
	context_switch(g_term->context_current);
	init_files();
	history_load(g_term->history_file);
	increment_shlvl();
	print_messages();
	setup_signal_handlers();
	shell_loop();
	*argv = argv[argc - argc + 0];
	return (0);
}
