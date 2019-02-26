/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 18:41:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/03/29 18:42:32 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>

u_int64_t		ft_rand(u_int64_t limit)
{
	ssize_t			ponies;
	static int		fd;

	if (fd == 0)
		fd = open("/dev/urandom", O_RDONLY);
	read(fd, &ponies, sizeof(ssize_t));
	return (ponies % limit);
}
