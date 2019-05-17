/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/31 14:45:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/10 18:04:57 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "line_editing.h"
#include "shell_job_control.h"
#include "twenty_one_sh.h"
#include "shell_history.h"
#include "shell_script.h"

struct s_term		*g_term;

static int			shell_loop(void)
{
	char		*commands;

	while (ponies_teleported())
	{
		g_interrupt = 0;
		if (g_term->input_state != STATE_NON_INTERACTIVE)
		{
			tcsetpgrp(0, jc_get()->shell_pid);
			TERM_APPLY_CONFIG(g_term->context_current->term_config);
			display_prompt(g_term->input_state = g_term->fallback_input_state);
			buff_clear(g_term->last_status = 0);
			commands = read_arbitrary();
			history_write(commands, get_history_fd());
		}
		else
			read_fd(0, &commands);
		run_script(tokenize(commands, TOKEN_DELIMITERS), false);
		ft_strdel(&commands);
		environ_push_entry(g_term->context_original->environ, "?",
			(commands = ft_itoa(g_term->last_status)), SCOPE_SHELL_LOCAL);
		ft_strdel(&commands);
		if (g_term->input_state == STATE_NON_INTERACTIVE)
			return (g_term->last_status);
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
	ft_bzero(host, sizeof(host));
	gethostname(host, 1024);
	set_env_v(vector, "HOST", host, SCOPE_SHELL_LOCAL);
	host[ft_strchr(host, '.') - host] = 0;
	set_env_v(vector, "SHORT_HOST", host, SCOPE_SHELL_LOCAL);
	set_env_v(vector, "BUILD", swap = ft_itoa(BUILD), SCOPE_SHELL_LOCAL);
	set_env_v(vector, "BUILD_DATE", BUILD_DATE, SCOPE_SHELL_LOCAL);
	set_env_v(vector, "SHELL", SH, SCOPE_EXPORT);
	ft_memdel((void **)&swap);
	environ_push_entry(vector, "$", (swap = ft_itoa(getpid())),
		SCOPE_SHELL_LOCAL);
	ft_memdel((void **)&swap);
	var = get_env_v(g_term->context_current->environ, "SHLVL");
	if (var == NULL || var->value == NULL || ft_strlen(var->value) == 0)
		set_env_v(g_term->context_current->environ, "SHLVL", "1", SCOPE_EXPORT);
	else
		set_env_v(g_term->context_current->environ, "SHLVL",
			(swap = ft_itoa((u_int32_t)ft_atoi(var->value) + 1)), SCOPE_EXPORT);
	ft_memdel((void **)&swap);
}

void				print_messages(void)
{
	t_var *var;

	if (g_term->input_state != STATE_NON_INTERACTIVE)
	{
		var = get_env_v(NULL, "TERM");
		if (!var || !var->value || tgetent(NULL, var->value) == ERR)
			ft_printf("\x1b[41;1m%-53s\x1b[0;0m\n\x1b[41;1m%53s\x1b[0;0m\n",
				"Warning: TERM environment variable is not set.",
				"Terminal capabilities are somewhat limited. ");
	}
}

void				run_file(const char *path)
{
	int		file;

	if (access(path, F_OK) == -1)
		ft_dprintf(2, ERR_NO_SUCH_FILE, path);
	else if (access(path, F_OK) != -1 && is_dir(path))
		ft_dprintf(2, ERR_IS_A_DIRECTORY, path);
	else if (access(path, F_OK) != -1 && access(path, R_OK) == -1)
		ft_dprintf(2, ERR_PERMISSION_DENIED, path);
	else if ((file = open(path, O_RDONLY)) == -1)
		ft_dprintf(2, ERR_NO_SUCH_FILE, path);
	else
	{
		dup2(file, 0);
		shell_loop();
		close(file);
	}
}

int					main(int argc, char **argv)
{
	init_shell_context();
	init_files();
	init_buffer_vector(MAX_INPUT);
	history_load(g_term->history_file);
	argv += parse_args(argc, argv);
	init_variables();
	jc_init(g_term->context_current);
	print_messages();
	setup_signal_handlers();
	if (argc == 1)
	{
		tcsetpgrp(0, jc_get()->shell_pid);
		shell_loop();
	}
	else
		run_file(argv[1]);
	return (g_term->last_status);
}
