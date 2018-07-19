/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_functions2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 19:13:14 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

/*
** Function splits string into 4-bit pieces and separates them using separator
*/

char	*apply_alternate_form_b(t_conv *conv, char **str, char separator)
{
	size_t	len;
	size_t	i;
	size_t	j;
	size_t	dummy;
	char	*swap;

	if (conv->alt_form == 0)
		return (*str);
	len = ft_strlen(*str);
	dummy = len % 4;
	if (dummy != 0)
		len += 4 - dummy;
	i = 0;
	j = 0;
	swap = ft_strnew(len += len / 4 - 1);
	while (i < 4 - dummy)
		swap[i++] = '0';
	while (i < len)
		if (i != 3 && (i - 4) % 5 == 0)
			swap[i++] = separator;
		else
			swap[i++] = (*str)[j++];
	ft_strdel(str);
	return (swap);
}

char	*apply_unicode_precision(t_conv *conv, char **str)
{
	char	*swap;
	size_t	i;

	if (conv->precision < 0)
		return (*str);
	swap = *str;
	i = conv->precision;
	while (swap[i] != '\0' && swap[i] & 0x80
			&& !((swap[i] & 0xC0) == 192
				|| (swap[i] & 0xE0) == 224
				|| (swap[i] & 0xF0) == 240))
	{
		i--;
	}
	if (conv->precision == 0 && conv->conv == 'c')
		i = 1;
	if (conv->precision >= 0)
	{
		swap = ft_strsub(swap, 0, i);
		ft_strdel(str);
	}
	return (swap);
}
