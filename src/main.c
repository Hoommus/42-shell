/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 16:11:51 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "shell_job_control.h"
#include "twenty_one_sh.h"
#include "shell_history.h"
#include "shell_script.h"

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
		g_term->last_status ? 31 : 32, g_term->last_status);
	return (size);
}

int					shell_loop(void)
{
	char		*commands;

	while (ponies_teleported())
	{
		g_interrupt = 0;
		g_term->last_status = 0;
		if (g_term->tty_fd != -1 && g_term->input_state
			!= STATE_NON_INTERACTIVE)
		{
			if (carpos_update(POS_CURRENT)->col > 1)
				ft_printf("\n");
			buff_clear(0);
			display_prompt(g_term->input_state = STATE_NORMAL);
			commands = read_arbitrary();
			history_write(commands, get_history_fd());
		}
		else
			read_fd(0, &commands);
		run_script(tokenize(commands, TOKEN_DELIMITERS), false);
		ft_strdel(&commands);
		if (g_term->input_state == STATE_NON_INTERACTIVE)
			exit(0);
	}
	return (0);
}

void				init_variables(void)
{
	t_env_vector	*vector;
	char			*swap;
	t_var			*var;
	char			host[1025];

	vector = g_term->context_original->environ;
	swap = ft_itoa(BUILD);
	ft_bzero(host, sizeof(host));
	gethostname(host, 1024);
	set_env_v(vector, "HOST", host, SCOPE_SHELL_LOCAL);
	host[ft_strchr(host, '.') - host] = 0;
	set_env_v(vector, "SHORT_HOST", host, SCOPE_SHELL_LOCAL);
	set_env_v(vector, "BUILD", swap, SCOPE_SHELL_LOCAL);
	set_env_v(vector, "BUILD_DATE", BUILD_DATE, SCOPE_SHELL_LOCAL);
	set_env_v(vector, "SHELL", SH, SCOPE_EXPORT);
	ft_memdel((void **)&swap);
	environ_push_entry(vector, "%", (swap = ft_itoa(getpid())), SCOPE_SHELL_LOCAL);
	ft_memdel((void **)&swap);
	var = get_env_v(g_term->context_current->environ, "SHLVL");
	if (var == NULL || var->value == NULL || ft_strlen(var->value) == 0)
		set_env_v(g_term->context_current->environ, "SHLVL", "1",
				SCOPE_EXPORT);
	else
		set_env_v(g_term->context_current->environ, "SHLVL",
				(swap = ft_itoa(ft_atoi(var->value) + 1)), SCOPE_EXPORT);
	ft_memdel((void **)&swap);
}

void				print_messages(void)
{
	t_var *var;

	if (g_term->input_state != STATE_NON_INTERACTIVE)
	{
		ft_printf("\n%*s\n%*s\n\n%*s%d (%s)\n",
			g_term->ws_col / 2 + 15, "  Willkommen und bienvenue.  ",
			g_term->ws_col / 2 + 15, "Welcome to 42sh divided by 2.",
			5, "Build #", BUILD, BUILD_DATE);
		var = get_env_v(NULL, "SHLVL");
		if (var && ft_atoi(var->value) > 2)
			ft_printf("\nRabbit hole depth: %s\n", var->value);
		var = get_env_v(NULL, "TERM");
		if (!var || !var->value || tgetent(NULL, var->value) == ERR)
			ft_printf("\x1b[41;1m%-53s\x1b[0;0m\n\x1b[41;1m%53s\x1b[0;0m\n",
				"Warning: TERM environment variable is not set.",
				"Terminal capabilities are somewhat limited.");
	}
}

int					main(int argc, char **argv)
{
	extern char		**environ;

	ft_printf("Initing...\n");
	init_shell_context();
	ft_printf("Initing files...\n");
	init_files();
	ft_printf("Loading history...\n");
	history_load(g_term->history_file);
	ft_printf("Parsing args...\n");
	parse_args(argc, argv);
	ft_printf("Initing variables...\n");
	init_variables();
	ft_printf("Initing job control...\n");
	jc_init(g_term->context_current);
	ft_printf("Printing messages...\n");
	print_messages();
	setup_signal_handlers();
	shell_loop();
	return (0);
}
