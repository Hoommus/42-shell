/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_alterators.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 15:45:40 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/07 18:48:55 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_script.h"

static int		g_rdr_error;

static int		open_at_fd(int what_fd, const char *path, int oflag,
	t_context *context)
{
	int		fd;

	if (!access(path, F_OK) && (access(path, R_OK) || access(path, W_OK)))
	{
		ft_dprintf(2, ERR_PERMISSION_DENIED, path);
		return (-(g_rdr_error = 1));
	}
	if (!access(path, F_OK) && is_dir(path))
	{
		ft_dprintf(2, ERR_IS_A_DIRECTORY, path);
		return (-(g_rdr_error = 1));
	}
	if ((fd = open_wrapper(path, oflag)) == -1 && (oflag & O_EXCL) == O_EXCL)
	{
		ft_dprintf(2, SH ": clobber error: `%s' exists\n", path);
		return (-(g_rdr_error = 1));
	}
	else if (fd == -1)
	{
		ft_dprintf(2, ERR_NO_SUCH_FILE, path);
		return (-(g_rdr_error = 1));
	}
	if (fd != what_fd)
		context_add_fd(context, what_fd, fd, path);
	return (what_fd);
}

static bool		is_fd_valid(t_context *context, int fd)
{
	struct s_fd_lst	*list;
	bool			is_valid;

	if (fd > MAX_FD || fd == -1)
		return (false);
	is_valid = false;
	list = context->fd_list;
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
	int		into_which_fd;
	int		what_to_dup_fd;

	if (rdr->right.path != NULL && ft_strcmp(rdr->right.path, "-"))
	{
		g_rdr_error = 1;
		return ((void)ft_dprintf(2, ERR_AMBIGUOUS_REDIRECT));
	}
	else if (rdr->right.path != NULL && !ft_strcmp(rdr->right.path, "-"))
		what_to_dup_fd = -1;
	else if (is_fd_valid(context, rdr->right.fd))
		what_to_dup_fd = rdr->right.fd;
	else
	{
		g_rdr_error = 1;
		return ((void)ft_dprintf(2, ERR_BAD_FD, rdr->right.fd));
	}
	into_which_fd = rdr->left.fd;
	context_add_fd(context, into_which_fd, what_to_dup_fd, "rdr_duped_1337");
}

int				alterate_filedes(const struct s_command *command,
	t_context *cntxt)
{
	const t_io_rdr	*rdr = (const t_io_rdr *)command->io_redirects - 1;

	g_rdr_error = 0;
	while (++rdr && rdr->type != TOKEN_NOT_APPLICABLE)
	{
		if (rdr->type == TOKEN_GREAT || rdr->type == TOKEN_DGREAT)
			open_at_fd(rdr->left.fd, rdr->right.path, O_CREAT | O_WRONLY |
				(rdr->type == TOKEN_DGREAT ? O_APPEND : O_TRUNC), cntxt);
		else if (rdr->type == TOKEN_CLOBBER)
			open_at_fd(rdr->left.fd, rdr->right.path, O_CREAT | O_WRONLY |
				O_EXCL, cntxt);
		else if (rdr->type == TOKEN_LESS)
			open_at_fd(rdr->left.fd, rdr->right.path, O_RDONLY, cntxt);
		else if (rdr->type == TOKEN_LESSGREAT)
			open_at_fd(rdr->left.fd, rdr->right.path, O_CREAT | O_RDWR, cntxt);
		else if (rdr->type == TOKEN_LESSAND || rdr->type == TOKEN_GREATAND)
			rdr_greatand(cntxt, rdr);
	}
	rdr_heredocs(cntxt, command->io_redirects);
	return (g_rdr_error);
}
