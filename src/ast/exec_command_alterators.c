/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_alterators.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 15:45:40 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/09 15:52:56 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "shell_script.h"
#include "shell_script_parser.h"

#include <assert.h>

int		open_at_fd(int what_fd, const char *path, int oflag, t_context *context)
{
	int		fd;

	fd = open_wrapper(path, oflag);
	if (fd != what_fd)
	{
		context_add_fd(context, what_fd, fd, path);
		return (what_fd);
	}
	return (fd);
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
	dup2_wrapper(which_fd, whereto_fd);
}

#pragma clang diagnostic pop

// TODO: Add access() checks
bool	alterate_filedes(const struct s_command *command, t_context *context)
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
					O_TRUNC | (rdr->type == TOKEN_DGREAT ? O_APPEND : 0), context);
		else if (rdr->type == TOKEN_LESS)
			open_at_fd(rdr->where.fd, rdr->what.path, O_RDONLY, context);
		else if (rdr->type == TOKEN_LESSGREAT)
			open_at_fd(rdr->where.fd, rdr->what.path, O_CREAT | O_RDWR, context);
		else if (rdr->type == TOKEN_DLESS || rdr->type == TOKEN_DLESSDASH)
			/* TODO */ ;
		else if (rdr->type == TOKEN_LESSAND)
			rdr_dup_into(rdr, TOKEN_LESSAND);
		else if (rdr->type == TOKEN_GREATAND)
			rdr_dup_into(rdr, TOKEN_GREATAND);
		rdr++;
	}
	return (rdr && rdr->type != TOKEN_IF && rdr->type != TOKEN_NOT_APPLICABLE);
}
