/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/06 16:03:23 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

void	eval(t_conv *conv, va_list *arg)
{
	if (conv->conv == 'd' || conv->conv == 'i')
		eval_di(conv, arg);
	else if (conv->conv == 'o' || conv->conv == 'x' || conv->conv == 'X'
			|| conv->conv == 'u')
		eval_uoxx(conv, arg);
	else if (conv->conv == 'c' || conv->conv == 's')
		eval_cs(conv, arg);
	else if (conv->conv == 'p')
		eval_p(conv, arg);
	else if (conv->conv == '%')
		eval_percent(conv);
	else if (conv->conv == 0 || ft_strchr(CONVERSIONS, conv->conv) == NULL)
		eval_invalid(conv);
	else if (conv->conv == 'b')
		eval_b(conv, arg);
	else if (conv->conv == 'n')
		eval_n(conv, arg);
}

/*
** Flags correspondence:
** hh = 1  (char)
** h  = 2  (short)
** l  = 4  (int)
** ll = 8  (long)
** j  = 16 (long long or something else)
** t  = 32 (long long or something else)
** z  = 64 (long long or something else)
*/

void	eval_di(t_conv *conv, va_list *arg)
{
	long long int	d;

	if (conv->modif & 64)
		d = va_arg(*arg, long long int);
	else if (conv->modif & 32)
		d = va_arg(*arg, ptrdiff_t);
	else if (conv->modif & 16)
		d = va_arg(*arg, intmax_t);
	else if (conv->modif & 8)
		d = va_arg(*arg, long long);
	else if (conv->modif & 4)
		d = va_arg(*arg, long);
	else if (conv->modif & 2)
		d = (short)va_arg(*arg, int);
	else if (conv->modif & 1)
		d = (signed char)va_arg(*arg, int);
	else
		d = va_arg(*arg, int);
	conv->res = itod(conv, d);
}

void	eval_uoxx(t_conv *conv, va_list *arg)
{
	unsigned long long int	d;

	if (conv->modif & 64)
		d = va_arg(*arg, size_t);
	else if (conv->modif & 32)
		d = (unsigned long long int)va_arg(*arg, ptrdiff_t);
	else if (conv->modif & 16)
		d = va_arg(*arg, unsigned long long int);
	else if (conv->modif & 8)
		d = va_arg(*arg, unsigned long long int);
	else if (conv->modif & 4)
		d = va_arg(*arg, unsigned long);
	else if (conv->modif & 2)
		d = (unsigned short)va_arg(*arg, unsigned int);
	else if (conv->modif & 1)
		d = (unsigned char)va_arg(*arg, unsigned int);
	else
		d = (unsigned int)va_arg(*arg, uintmax_t);
	override_flags(conv, d);
	if (conv->conv == 'o')
		conv->res = itoo(conv, d);
	else if (conv->conv == 'x' || conv->conv == 'X')
		conv->res = itox(conv, d);
	else if (conv->conv == 'u')
		conv->res = itou(conv, d);
}

void	eval_p(t_conv *conv, va_list *arg)
{
	unsigned long long	nbr;

	nbr = (unsigned long long)va_arg(*arg, void *);
	conv->alt_form = '#';
	conv->conv = 'p';
	conv->modif = conv->modif | 8;
	conv->res = itox(conv, nbr);
}

void	eval_invalid(t_conv *conv)
{
	conv->res = apply_generic_precision(conv, &conv->res, ft_strlen(conv->res));
	conv->res = apply_generic_width(conv, &conv->res, ft_strlen(conv->res), 0);
}
