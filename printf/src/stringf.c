/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/21 18:35:00 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

void	eval_cs(t_conv *conv, va_list *arg)
{
	char	*str;
	char	*ar;

	if (conv->conv == 's')
	{
		if (conv->modif & 4 || conv->modif & 8)
			str = to_unicode_string(va_arg(*arg, wchar_t *));
		else
		{
			ar = va_arg(*arg, char *);
			str = ar == NULL ? ft_strdup("(null)") : ft_strdup(ar);
		}
	}
	else
	{
		if (conv->modif & 4 || conv->modif & 8)
			str = to_unicode((wchar_t)va_arg(*arg, long));
		else
			str = to_unicode((char)va_arg(*arg, int));
	}
	if (str == NULL)
		str = ft_strdup("(null)");
	str = apply_unicode_precision(conv, &str);
	str = apply_generic_width(conv, &str, ft_strlen(str), 0);
	conv->res = str;
}

void	eval_percent(t_conv *conv)
{
	char		*str;

	str = ft_strdup("%");
	conv->alt_form = 0;
	str = apply_generic_precision(conv, &str, ft_strlen(str));
	str = apply_generic_width(conv, &str, ft_strlen(str), 0);
	conv->res = str;
}
