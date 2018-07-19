/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 15:51:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

int		ft_printf_fd(int fd, const char *restrict format, va_list *lst)
{
	if (fd <= 0)
		return (-1);
	g_symbols = 0;
	g_written = 0;
	g_error = 0;
	g_output = fd;
	find_eval_print((char *)format, lst);
	bufferize(NULL, -1);
	return (g_error == 0 ? g_symbols : -1);
}

int		ft_dprintf(int fd, const char *restrict format, ...)
{
	va_list		list;

	g_symbols = 0;
	va_start(list, format);
	ft_printf_fd(fd, format, &list);
	va_end(list);
	return (g_error == 0 ? g_symbols : -1);
}
