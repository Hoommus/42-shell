/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_alterators.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 15:45:40 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 17:04:06 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

static int		g_access_error;

static int		open_at_fd(int what_fd, const char *path, int oflag,
	t_context *context)
{
	int		fd;

	if (!access(path, F_OK) && (access(path, R_OK) || access(path, W_OK)))
	{
		ft_dprintf(2, ERR_PERMISSION_DENIED, path);
		return (-(g_access_error = 1));
	}
	if (!access(path, F_OK) && is_dir(path))
	{
		ft_dprintf(2, ERR_IS_A_DIRECTORY, path);
		return (-(g_access_error = 1));
	}
	fd = open_wrapper(path, oflag);
	if (fd != what_fd)
	{
		context_add_fd(context, what_fd, fd, path);
		return (what_fd);
	}
	return (fd);
}

bool			is_fd_valid(int fd)
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

static void		rdr_greatand(t_context *context, const t_io_rdr *rdr)
{
	int		which_fd;
	int		whereto_fd;

	if (rdr->where.path != NULL)
	{
		if (access(rdr->where.path, W_OK) != 0)
		{
			g_access_error = 1;
			return ((void)ft_dprintf(2, ERR_PERMISSION_DENIED, rdr->what.path));
		}
		else
			whereto_fd = open(rdr->where.path, O_CREAT | O_TRUNC | O_WRONLY);
	}
	else if (is_fd_valid(rdr->where.fd))
		whereto_fd = rdr->where.fd;
	else
	{
		g_access_error = 1;
		return ((void)ft_dprintf(2, ERR_BAD_FD, rdr->where.fd));
	}
	which_fd = rdr->what.fd;
	context_add_fd(context, whereto_fd, which_fd, "rdr_duped");
}

static void		rdr_lessand(t_context *context, const t_io_rdr *rdr)
{
	int		which_fd;
	int		whereto_fd;

	if (rdr->what.path != NULL)
	{
		if (access(rdr->what.path, F_OK) || access(rdr->what.path, R_OK))
			g_access_error = 1;
		if (access(rdr->what.path, F_OK) != 0)
			return ((void)ft_dprintf(2, ERR_NO_SUCH_FILE, rdr->what.path));
		if (access(rdr->what.path, R_OK) != 0)
			return ((void)ft_dprintf(2, ERR_PERMISSION_DENIED, rdr->what.path));
		else
			which_fd = open(rdr->what.path, O_RDONLY);
	}
	else if (is_fd_valid(rdr->what.fd))
		which_fd = rdr->what.fd;
	else
	{
		g_access_error = 1;
		return ((void)ft_dprintf(2, ERR_BAD_FD, rdr->where.fd));
	}
	whereto_fd = rdr->where.fd;
	context_add_fd(context, which_fd, whereto_fd, "rdr_duped");
}

int				alterate_filedes(const struct s_command *command,
	t_context *contxt)
{
	const t_io_rdr	*rdr = (const t_io_rdr *)command->io_redirects;

	g_access_error = 0;
	while (rdr && rdr->type != TOKEN_IF && rdr->type != TOKEN_NOT_APPLICABLE)
	{
		if (rdr->type == TOKEN_GREATAND && !ft_strcmp(rdr->where.path, "-"))
			context_mark_fd_closed(contxt, rdr->what.fd, true);
		else if (rdr->type == TOKEN_LESSAND && !ft_strcmp(rdr->what.path, "-"))
			context_mark_fd_closed(contxt, rdr->where.fd, true);
		else if (rdr->type == TOKEN_GREAT || rdr->type == TOKEN_CLOBBER ||
				rdr->type == TOKEN_DGREAT)
			open_at_fd(rdr->what.fd, rdr->where.path, O_CREAT | O_WRONLY |
				(rdr->type == TOKEN_DGREAT ? O_APPEND : O_TRUNC), contxt);
		else if (rdr->type == TOKEN_LESS)
			open_at_fd(rdr->where.fd, rdr->what.path, O_RDONLY, contxt);
		else if (rdr->type == TOKEN_LESSGREAT)
			open_at_fd(rdr->where.fd, rdr->what.path, O_CREAT | O_RDWR, contxt);
		else if (rdr->type == TOKEN_LESSAND)
			rdr_greatand(contxt, rdr);
		else if (rdr->type == TOKEN_GREATAND)
			rdr_lessand(contxt, rdr);
		rdr++;
	}
	rdr_heredocs(contxt, command->io_redirects);
	return (g_access_error);
}
