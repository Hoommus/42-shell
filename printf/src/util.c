/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/04 12:59:36 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 17:10:29 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

t_conv	*create_empty(void)
{
	t_conv	*conv;

	conv = (t_conv *)malloc(sizeof(t_conv));
	if (conv == NULL)
		return (NULL);
	conv->format_offset = 0;
	conv->zero_padding = 0;
	conv->min_width = -1;
	conv->precision = -1;
	conv->long_afeg = 0;
	conv->pad_dir = '+';
	conv->modif = 0;
	conv->conv = 0;
	conv->sign = 0;
	conv->res = NULL;
	return (conv);
}

void	free_conv(t_conv **conv)
{
	if (conv == NULL || *conv == NULL)
		return ;
	ft_bzero(*conv, sizeof(conv));
	free((*conv)->res);
	free(*conv);
	*conv = NULL;
}

void	find_eval_print(char *format, va_list *list)
{
	char		*percent;
	t_conv		*conv;

	while (*format)
	{
		percent = ft_strchr(format, '%');
		if (percent > format)
		{
			bufferize(format, percent - format);
			format += percent - format;
		}
		else if (percent == NULL)
		{
			bufferize(format, ft_strlen(format));
			break ;
		}
		bufferize((conv = resolve(percent, list))->res, -1);
		format += conv->format_offset != 0 ? conv->format_offset : 1;
		free_conv(&conv);
	}
}
