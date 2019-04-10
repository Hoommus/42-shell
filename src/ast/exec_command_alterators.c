/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_alterators.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 15:45:40 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/13 12:27:12 by vtarasiu         ###   ########.fr       */
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

	if (fd > MAX_FD || fd == -1)
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

void	rdr_greatand(t_context *context, const t_io_rdr *rdr)
{
	int		which_fd;
	int		whereto_fd;

	if (rdr->where.path != NULL)
	{
		if (access(rdr->where.path, W_OK) != 0)
		{
			exec_abort(ft_dprintf(2, "21sh: permission denied: %s\n",
				rdr->what.path));
			return ;
		}
		else
			whereto_fd = open(rdr->where.path, O_CREAT | O_TRUNC | O_WRONLY);
	}
	else if (is_fd_valid(rdr->where.fd))
		whereto_fd = rdr->where.fd;
	else
	{
		exec_abort(ft_dprintf(2, "21sh: Bad file descriptor: %d\n",
			rdr->where.fd));
		return ;
	}
	which_fd = rdr->what.fd;
	context_add_fd(context, which_fd, whereto_fd, "rdr_duped");
}

void	rdr_lessand(t_context *context, const t_io_rdr *rdr)
{
	int		which_fd;
	int		whereto_fd;

	if (rdr->what.path != NULL)
	{
		if (access(rdr->what.path, F_OK | R_OK) != 0)
		{
			exec_abort(ft_dprintf(2, "21sh: permission denied: %s\n",
				rdr->what.path));
			return ;
		}
		else
			which_fd = open(rdr->what.path, O_RDONLY);
	}
	else if (is_fd_valid(rdr->what.fd))
		which_fd = rdr->what.fd;
	else
	{
		exec_abort(ft_dprintf(2, "21sh: Bad file descriptor: %d\n",
			rdr->what.fd));
		return ;
	}
	whereto_fd = rdr->where.fd;
	context_add_fd(context, which_fd, whereto_fd, "rdr_duped");
}

// TODO: Add access() checks
bool	alterate_filedes(const struct s_command *command, t_context *context)
{
	const t_io_rdr	*rdr = (const t_io_rdr *)command->io_redirects;

	while (rdr && rdr->type != TOKEN_IF && rdr->type != TOKEN_NOT_APPLICABLE)
	{
		if (rdr->type == TOKEN_GREATAND && !ft_strcmp(rdr->where.path, "-"))
			context_mark_fd_closed(context, rdr->what.fd, true);
		else if (rdr->type == TOKEN_LESSAND && !ft_strcmp(rdr->what.path, "-"))
			context_mark_fd_closed(context, rdr->where.fd, true);
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
			rdr_greatand(context, rdr);
		else if (rdr->type == TOKEN_GREATAND)
			rdr_lessand(context, rdr);
		rdr++;
	}
	return (rdr && rdr->type != TOKEN_IF && rdr->type != TOKEN_NOT_APPLICABLE);
}
