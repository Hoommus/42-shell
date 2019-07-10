/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 12:28:13 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 17:09:36 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_environ.h"
#include "line_editing.h"

void			init_shell_context(void)
{
	extern const char	**environ;
	extern t_env_vector	*g_alias_vector;

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
	g_term->context_current = context_duplicate(g_term->context_original,
		XDUP_ENV | XDUP_FDS);
	environ_deallocate_vector(g_term->context_current->environ);
	free(g_term->context_current->term_config);
	g_term->context_current->environ = g_term->context_original->environ;
	g_term->context_current->term_config = init_term();
	g_alias_vector = environ_create_vector(8);
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
	t_var	*var;

	var = get_env_v(NULL, "HISTFILE");
	if (var == NULL || var->value == NULL || ft_strlen(var->value) == 0)
	{
		g_term->history_file = init_fd_at_home(HISTORY_FILE, 0);
		set_env_v(g_term->context_current->environ, "HISTFILE",
			HISTORY_FILE, SCOPE_SHELL_LOCAL);
	}
	else
		g_term->history_file = init_fd_at_home(var->value, 0);
}

int				parse_args(int argc, char **argv)
{
	environ_push_entry(g_term->context_original->environ, "0",
		argv[0], SCOPE_SHELL_LOCAL);
	if (argc > 1)
	{
		g_term->input_state = STATE_NON_INTERACTIVE;
		g_term->fallback_input_state = STATE_NON_INTERACTIVE;
	}
	return (0);
}
