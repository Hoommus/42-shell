/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:19:45 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/03/29 14:27:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		magic(char **line, char **buf)
{
	char	*swap;
	char	*slash;

	slash = ft_strchr(*buf, '\n');
	if (slash == NULL && ft_strlen(*buf) > 0)
	{
		*line = ft_strdup(*buf);
		ft_strdel(buf);
		return (1);
	}
	else if (ft_strlen(*buf) > 0)
	{
		*line = ft_strsub(*buf, 0, slash - *buf);
		swap = *buf;
		*buf = ft_strsub(*buf, slash - *buf + 1, ft_strlen(*buf));
		ft_strdel(&swap);
		return (1);
	}
	else
	{
		ft_strdel(buf);
		return (0);
	}
}

int		get_next_line(const int fd, char **line)
{
	static char	*buf[4864];
	char		*b;
	char		*swap;
	long long	cat;

	if (fd < 0 || fd >= 4864 || line == NULL)
		return (-1);
	buf[fd] = buf[fd] == NULL ? ft_strnew(0) : buf[fd];
	b = ft_strnew(BUFF_SIZE);
	while (ft_strchr(buf[fd], '\n') == 0 && (cat = read(fd, b, BUFF_SIZE)) != 0)
	{
		if (fd < 0 || cat == -1 || b == 0)
			return (-1);
		swap = buf[fd];
		buf[fd] = ft_strjoin(buf[fd], b);
		ft_strdel(&swap);
		ft_strdel(&b);
		b = ft_strnew(BUFF_SIZE);
	}
	ft_strdel(&b);
	return (magic(line, &(buf[fd])));
}
