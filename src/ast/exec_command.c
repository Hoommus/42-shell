/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/19 17:05:23 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

int		open_at_fd(int what_fd, const char *path, int oflag)
{
	int		fd;

	fd = open_wrapper(path, oflag);
	if (fd != what_fd)
	{
		dup2(fd, what_fd);
		close_wrapper(fd);
		return (what_fd);
	}
	return (fd);
}

void	alterate_vars(const struct s_command *command, t_context *context)
{
	const char	**assignments = (const char **)command->assignments;
	char		**splitted;
	int			scope;
	int			i;

	if (command->args == NULL || command->args[0] == NULL)
		scope = VAR_SHELL_LOCAL;
	else
		scope = VAR_COMMAND_LOCAL;
	i = 0;
	while (assignments && assignments[i])
	{
		splitted = smart_split(assignments[i], "=");
		environ_push_entry(context->environ, splitted[0], splitted[1], scope);
		free_array((void **)splitted);
		i++;
	}
}

void	alterate_filedes(const struct s_command *command)
{
	const t_io_rdr	**rdrs = (const t_io_rdr **)command->io_redirects;
	int				oflag;
	int				i;

	i = 0;
	while (rdrs && rdrs[i])
	{
		oflag = O_CREAT | (rdrs[i]->append ? O_APPEND : O_TRUNC);
		if (rdrs[i]->type == TOKEN_GREATAND && ft_strcmp(rdrs[i]->where.string, "-") == 0)
			close_wrapper(rdrs[i]->what.fd);
		else if (rdrs[i]->type == TOKEN_LESSAND && ft_strcmp(rdrs[i]->what.string, "-") == 0)
			close_wrapper(rdrs[i]->where.fd);
		else if (rdrs[i]->type == TOKEN_GREAT
				|| rdrs[i]->type == TOKEN_CLOBBER
				|| rdrs[i]->type == TOKEN_DGREAT)
			open_at_fd(rdrs[i]->what.fd, rdrs[i]->where.string, oflag | O_WRONLY);
		else if (rdrs[i]->type == TOKEN_LESS)
			open_at_fd(rdrs[i]->where.fd, rdrs[i]->what.string, oflag | O_RDONLY);
		else if (rdrs[i]->type == TOKEN_LESSGREAT)
			open_at_fd(rdrs[i]->where.fd, rdrs[i]->what.string, oflag | O_RDWR);
		else if (rdrs[i]->type == TOKEN_DLESS || rdrs[i]->type == TOKEN_DLESSDASH)
			/* TODO */ ;
		else if (rdrs[i]->type == TOKEN_LESSAND)
			/* TODO */ ;
		else if (rdrs[i]->type == TOKEN_GREATAND)
			/* TODO */ ;
		else
			abort();
		i++;
	}
}

/*
** If new_context is not NULL, it must have altered stdin and / or stdout,
** if it was called from pipeline executor
*/

int		exec_command(const t_node *command_node, t_context *new_context)
{
	const struct s_command	*command = command_node->command;
	t_context				*context;
	int						status;

	if (command_node->node_type != NODE_COMMAND)
	{
		ft_printf("assumed command_node is not actually command\n");
		abort();
	}
	if (new_context)
		context = new_context;
	else
		context = context_duplicate(g_term->context_original, true);
	context_switch(context);
	// TODO: Expand environment variables in args, assignments et cetera
	alterate_vars(command, context);
	alterate_filedes(command);

	status = execute(command->args);

	// TODO: Create string transformations pipeline framework
	// TODO: Solve expansions and globs
	// TODO: Remove quotes
	// TODO: Expand backqoutes and $()
	// TODO: Remove backslashes from escaped symbols

	// TODO: Execute command / create and run a job
	context_deep_free(&context);
	context_switch(NULL);
	return (status);
}


