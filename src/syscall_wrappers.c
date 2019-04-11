/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrappers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:15:51 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/28 16:54:15 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

// TODO: Consider adding a universal read wrapper

int		openm_wrapper(const char *path, int oflag, mode_t mode)
{
	int		fd;

	fd = open(path, oflag, mode);
	if (fd == -1)
		ft_dprintf(2, "21sh: some weird 'open' error\n"
								"%s \noflag: %x mode: %x", path, oflag, mode);
	if (fd != -1)
		context_add_fd(g_term->context_current, fd, fd, path);
	return (fd);
}

int		open_wrapper(const char *path, int oflag)
{
	return (openm_wrapper(path, oflag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
}

int		close_wrapper(int filedes)
{
	context_remove_fd(g_term->context_current, filedes);
	return (close(filedes));
}

int		dup2_wrapper(int fd_what, int fd_where)
{
	int		status;

	status = dup2(fd_what, fd_where);
	if (status == -1)
		ft_dprintf(2, "21sh: some weird 'dup2' error"
				"\nwhat: %d where: %d\n", fd_what, fd_where);
	context_add_fd(g_term->context_current, fd_where, fd_what, "duped2");
	return (status);
}

int		dup_wrapper(int fd_what)
{
	int		new;

	new = dup(fd_what);
	if (new == -1)
		ft_dprintf(2, "21sh: some weird 'dup2' error"
					  "\nwhat: %d\n", fd_what);
	context_add_fd(g_term->context_current, fd_what, new, "duped");
	return (new);
}
