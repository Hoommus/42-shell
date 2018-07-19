/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evals2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/16 18:37:12 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

char	*itob(t_conv *conv, unsigned long long nbr)
{
	char	*new;

	if (nbr == 0 && conv->precision == 0 && conv->alt_form == 0)
		new = ft_strnew(0);
	else
		new = utos_base(nbr, 2, ITOA_LOWER);
	new = apply_generic_precision(conv, &new, ft_strlen(new));
	if (conv->zero_padding != 0 && (conv->pad_dir == '-'
			|| conv->precision != -1))
		conv->zero_padding = 0;
	new = apply_alternate_form_b(conv, &new, ' ');
	new = apply_generic_width(conv, &new, ft_strlen(new), 0);
	return (new);
}

void	eval_b(t_conv *conv, va_list *arg)
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
	conv->res = itob(conv, d);
}

void	eval_n(t_conv *conv, va_list *arg)
{
	void *d;

	if (conv->modif & 64)
		d = va_arg(*arg, long long int *);
	else if (conv->modif & 32)
		d = va_arg(*arg, ptrdiff_t *);
	else if (conv->modif & 16)
		d = va_arg(*arg, intmax_t *);
	else if (conv->modif & 8)
		d = va_arg(*arg, long long *);
	else if (conv->modif & 4)
		d = va_arg(*arg, long *);
	else if (conv->modif & 2)
		d = (short *)va_arg(*arg, int *);
	else if (conv->modif & 1)
		d = (signed char *)va_arg(*arg, int *);
	else
		d = va_arg(*arg, int *);
	*((int *)d) = g_symbols;
	conv->res = ft_strnew(0);
}
