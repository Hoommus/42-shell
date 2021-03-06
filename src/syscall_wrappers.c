/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall_wrappers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 13:15:51 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/25 18:17:40 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int		openm_wrapper(const char *path, int oflag, mode_t mode)
{
	int		fd;
	int		flags;

	fd = open(path, oflag, mode);
	if (fd != -1)
	{
		context_add_fd(g_term->context_current, fd, fd, path);
		flags = fcntl(fd, F_GETFL);
		if (flags != -1)
			fcntl(fd, F_SETFL, flags | O_CLOEXEC);
	}
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
