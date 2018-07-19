/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integerf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/14 17:43:24 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:20 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

char	*trimxx(t_conv *conv, char **str)
{
	size_t	i;
	char	*swap;

	swap = *str;
	i = ft_strlen(*str);
	if (i > 8 && conv->modif < 4)
	{
		swap = ft_strsub(*str, i - 8, i);
		ft_strdel(str);
	}
	if (ft_strequ(swap, "00000000"))
	{
		ft_strdel(&swap);
		swap = ft_strdup("0");
	}
	return (swap);
}

/*
** TODO: Fix those awful crutches here
*/

char	*itox(t_conv *conv, unsigned long long nbr)
{
	char	*new;

	if ((nbr == 0 && conv->precision == 0) ||
			(conv->conv == 'p' && conv->precision == 0))
		new = ft_strnew(0);
	else
		new = utos_base(nbr, 16, conv->conv == 'X' ? ITOA_UPPER : ITOA_LOWER);
	if (conv->modif < 4 && conv->modif != 0)
		new = trimxx(conv, &new);
	new = apply_generic_precision(conv, &new, ft_strlen(new));
	if (conv->alt_form != 0 && conv->min_width > -1 && conv->zero_padding != 0)
	{
		new = apply_generic_width(conv, &new, ft_strlen(new), 0);
		if ((conv->alt_form != 0 && nbr != 0) || conv->conv == 'p')
			new = apply_alternate_form_ox(conv, &new);
	}
	else
	{
		if (nbr != 0 || conv->conv == 'p')
			new = apply_alternate_form_ox(conv, &new);
		new = apply_generic_width(conv, &new, ft_strlen(new), 0);
	}
	return (new);
}

char	*itoo(t_conv *conv, unsigned long long nbr)
{
	char	*new;

	if (nbr == 0 && conv->precision == 0 && conv->alt_form == 0)
		new = ft_strnew(0);
	else
		new = utos_base(nbr, 8, ITOA_LOWER);
	new = apply_generic_precision(conv, &new, ft_strlen(new));
	if (nbr != 0)
		new = apply_alternate_form_ox(conv, &new);
	if (conv->precision != -1)
		new = apply_generic_width(conv, &new, ft_strlen(new), ' ');
	else
		new = apply_generic_width(conv, &new, ft_strlen(new), 0);
	return (new);
}

char	*itou(t_conv *conv, unsigned long long nbr)
{
	char	*new;

	if (conv->precision == 0 && nbr == 0)
		new = ft_strnew(0);
	else
		new = utos_base(nbr, 10, ITOA_LOWER);
	new = apply_generic_precision(conv, &new, ft_strlen(new));
	if (nbr != 0)
		new = apply_alternate_form_ox(conv, &new);
	if (conv->precision != -1)
		new = apply_generic_width(conv, &new, ft_strlen(new), ' ');
	else
		new = apply_generic_width(conv, &new, ft_strlen(new), 0);
	return (new);
}

char	*itod(t_conv *conv, long long int nbr)
{
	char	*new;

	if (conv->precision == 0 && nbr == 0 && conv->sign == 0)
		new = ft_strnew(0);
	else
		new = nbr == LLONG_MIN ? ft_strdup("9223372036854775808")
								: itos_base(ABS(nbr), 10);
	override_flags(conv, nbr);
	new = apply_generic_precision(conv, &new, ft_strlen(new));
	if (conv->zero_padding == 0)
	{
		new = apply_sign(conv, &new, nbr < 0 ? -1 : 1);
		new = apply_space(conv, &new);
		new = apply_generic_width(conv, &new, ft_strlen(new), 0);
	}
	else
	{
		new = apply_generic_width(conv, &new, ft_strlen(new), 0);
		new = apply_sign(conv, &new, nbr < 0 ? -1 : 1);
		new = apply_space(conv, &new);
	}
	return (new);
}
