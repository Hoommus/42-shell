/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 17:50:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/30 13:53:35 by vtarasiu         ###   ########.fr       */
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
		dup2_wrapper(fd, what_fd);
		close_wrapper(fd);
		return (what_fd);
	}
	return (fd);
}

char	**split_to_var(const char *str)
{
	t_token		*swap;
	t_token		*tokens;
	char		**splitted;

	tokens = tokenize((char *)str, "=");
	splitted = ft_memalloc(sizeof(char *) * 3);
	splitted[0] = (char *)(tokens->value);
	if (tokens->next)
		splitted[1] = (char *)(tokens->next->value);
	else
		splitted[1] = NULL;
	while (tokens)
	{
		swap = tokens->next;
		free(tokens);
		tokens = swap;
	}
	return (splitted);
}

/*
** Saves resources by closing unnecessary fds, which are already duplicated;
*/
void	close_redundant_filedes(t_context *context)
{
	struct s_fd_lst	*list;

	list = context->fd_list;
	while (list)
	{
		if (list->original != list->current)
		{
			close(list->current);
			list->current = list->original;
		}
		list = list->next;
	}
}

bool	is_fd_valid(int fd)
{
	struct s_fd_lst	*list;
	bool			is_valid;

	if (fd > MAX_FD)
		return (false);
	is_valid = false;
	list = g_term->context_current->fd_list;
	while (list && !is_valid)
	{
		if (list->original == fd)
			is_valid = true;
		list = list->next;
	}
	return (is_valid);
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuninitialized"

// TODO: Find a way of improving this repetitive code

void	rdr_dup_into(const t_io_rdr *rdr, enum e_token_type type)
{
	int		which_fd;
	int		whereto_fd;

	if (type == TOKEN_GREATAND)
	{
		if (rdr->where.path != NULL)
			whereto_fd = open_wrapper(rdr->where.path, O_CREAT | O_TRUNC | O_WRONLY);
		else if (is_fd_valid(rdr->where.fd))
			whereto_fd = rdr->where.fd;
		else
			exec_abort(ft_dprintf(2, "21sh: Bad file descriptor: %d\n", rdr->where.fd));
		which_fd = rdr->what.fd;
	}
	else if (type == TOKEN_LESSAND)
	{
		if (rdr->what.path != NULL)
			which_fd = open_wrapper(rdr->what.path,  O_RDONLY);
		else if (is_fd_valid(rdr->what.fd))
			which_fd = rdr->what.fd;
		else
			exec_abort(ft_dprintf(2, "21sh: Bad file descriptor: %d\n", rdr->what.fd));
		whereto_fd = rdr->where.fd;
	}
	else
		abort();
	dup2_wrapper(which_fd, whereto_fd);
}

#pragma clang diagnostic pop

void	alterate_vars(const struct s_command *command, t_context *context)
{
	const char	**assignments = (const char **)command->assignments;
	char		**splitted;
	int			scope;
	int			i;

	if (command->args == NULL || command->args[0] == NULL)
		scope = SCOPE_SHELL_LOCAL;
	else
		scope = SCOPE_COMMAND_LOCAL;
	i = 0;
	while (assignments && assignments[i])
	{
		splitted = split_to_var(assignments[i]);
		environ_push_entry(context->environ, splitted[0], splitted[1], scope);
		free_array((void **)splitted);
		i++;
	}
}

//void	resolve_heredoc(t_io_rdr *rdr)
//{
//	int		pipe_fds[2];
//
//	pipe(pipe_fds);
//
//}
// TODO: Add access() checks
void	alterate_filedes(const struct s_command *command)
{
	const t_io_rdr	*rdr = (const t_io_rdr *)command->io_redirects;

	while (rdr && rdr->type != TOKEN_IF && rdr->type != TOKEN_NOT_APPLICABLE)
	{
		if (rdr->type == TOKEN_GREATAND && !ft_strcmp(rdr->where.path, "-"))
			close_wrapper(rdr->what.fd);
		else if (rdr->type == TOKEN_LESSAND && !ft_strcmp(rdr->what.path, "-"))
			close_wrapper(rdr->where.fd);
		else if (rdr->type == TOKEN_GREAT || rdr->type == TOKEN_CLOBBER
				 || rdr->type == TOKEN_DGREAT)
			open_at_fd(rdr->what.fd, rdr->where.path, O_CREAT | O_WRONLY |
						O_TRUNC | (rdr->type == TOKEN_DGREAT ? O_APPEND : 0));
		else if (rdr->type == TOKEN_LESS)
			open_at_fd(rdr->where.fd, rdr->what.path, O_RDONLY);
		else if (rdr->type == TOKEN_LESSGREAT)
			open_at_fd(rdr->where.fd, rdr->what.path, O_CREAT | O_RDWR);
		else if (rdr->type == TOKEN_DLESS || rdr->type == TOKEN_DLESSDASH)
			/* TODO */ ;
		else if (rdr->type == TOKEN_LESSAND)
			rdr_dup_into(rdr, TOKEN_LESSAND);
		else if (rdr->type == TOKEN_GREATAND)
			rdr_dup_into(rdr, TOKEN_GREATAND);
		rdr++;
	}
}

void	expand_everything(struct s_command *command)
{
	const struct s_command	dummy = *command;
	char					*swap;
	int						i;

	i = -1;
	while (dummy.args[++i])
	{
		swap = dummy.args[i];
		dummy.args[i] = expand(dummy.args[i]);
		ft_memdel((void **)&swap);
	}
}

/*
** If new_context is not NULL, it must have altered stdin and / or stdout,
** if it was called from pipeline executor
**
** The incapsulation of command execution is fucked while inside pipe, because
** Bash script is stupid as fuck and I wish there was better script language.
** See alterate_filedes() function, which also must be called before pipe exec.
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
	// TODO: backup and close all existing filedes before switch
	context_switch(context);
	// TODO: Expand environment variables in args, assignments et cetera
	expand_everything((struct s_command *)command);
	alterate_vars(command, context);
	if (!new_context)
		alterate_filedes(command);

	// TODO: do something with bare assignments in command
	if (!g_is_interrupted)
		status = execute(command->args);
	else
		status = 0;
	if (status != 0)
		ft_dprintf(2, "21sh: exec: command not found: %s\n", command->args[0]);
	// TODO: Create string transformations pipeline framework
	// TODO: Remove quotes
	// TODO: Expand backqoutes and $()
	// TODO: Remove backslashes from escaped symbols

	context_deep_free(&context);
	context_switch(NULL);
	return (status);
}
