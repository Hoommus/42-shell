/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 12:15:28 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf.h"
#include "../include/ft_printf_internal.h"

t_conv	*resolve(char *str, va_list *arg)
{
	t_conv	*conv;
	char	*copy;

	copy = str++;
	conv = create_empty();
	if (ft_strlen(str) == 0)
		return (conv);
	str += find_flags(str, conv, arg);
	str += set_modifier_bits(str, conv);
	str += guess_convertion(str, conv);
	if (conv->conv == 0)
		conv->res = to_unicode(*str++);
	conv->format_offset = str < copy ? copy - str : str - copy;
	eval(conv, arg);
	return (conv);
}

void	bufferize(char *s, long long len)
{
	static unsigned char	buffer[BUFFER_SIZE];
	static unsigned long	i;

	len = len < 0 ? ft_strlen(s) : len;
	while (i < BUFFER_SIZE && len--)
	{
		buffer[i++] = (unsigned char)*s == 1 ? 0 : *s;
		s++;
		g_symbols++;
	}
	if (s == NULL && len == -1)
	{
		g_written += write(g_output, buffer, i);
		i = 0;
	}
	else if (i == BUFFER_SIZE)
	{
		g_written += write(g_output, buffer, i);
		ft_bzero(buffer, BUFFER_SIZE);
		i = 0;
	}
	if (len > 0)
		bufferize(s, len);
}

int		ft_printf(const char *restrict format, ...)
{
	va_list		list;

	g_symbols = 0;
	va_start(list, format);
	ft_printf_fd(1, format, &list);
	va_end(list);
	return (g_symbols);
}
