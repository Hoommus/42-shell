/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/15 16:41:29 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/07/01 15:06:19 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_printf_internal.h"

void	guess_flag(char c, t_conv *conv)
{
	if (c == '0')
		conv->zero_padding = 'z';
	else if (ft_isdigit(c))
		return ;
	else if (c == '#')
		conv->alt_form = '#';
	else if (c == '-')
		conv->pad_dir = '-';
	else if (c == ' ')
		conv->space = ' ';
	else if (c == '+')
		conv->sign = '+';
	else if (c == 'L')
		conv->long_afeg = 'L';
	else if (c == '\'')
		conv->apostrophe = '\'';
}

int		find_flags(char *str, t_conv *conv, va_list *list)
{
	int		i;

	i = 0;
	while (str[i] && ft_strchr(FLAGS, str[i]))
	{
		guess_flag(str[i], conv);
		if ((ft_isdigit(str[i]) || str[i] == '*') && str[i] != '0'
			&& str[i - 1] != '.')
			i += resolve_wildcard_or_else(conv, str + i, list, FALSE);
		if (str[i] == '.')
			i += resolve_wildcard_or_else(conv, str + i, list, TRUE);
		i++;
	}
	return (i);
}

int		guess_convertion(char *str, t_conv *conv)
{
	if (ft_strchr(CONVERSIONS, *str))
		conv->conv = *str;
	if (conv->conv == 'D' || conv->conv == 'O' || conv->conv == 'U'
		|| conv->conv == 'C' || conv->conv == 'S' || conv->conv == 'B')
	{
		conv->modif = conv->modif | 12;
		conv->conv = (char)ft_tolower(conv->conv);
	}
	return (conv->conv == 0 ? 0 : 1);
}

int		set_modifier_bits(char *str, t_conv *conv)
{
	int		i;

	i = 0;
	while (ft_strchr(MODIFIERS, str[i]) != NULL)
	{
		if (ft_strnstr(str + i, "ll", 2) && ++i)
			conv->modif |= 8;
		else if (ft_strnstr(str + i, "hh", 2) && ++i)
			conv->modif |= 1;
		else if (*str == 'h')
			conv->modif |= 2;
		else if (*str == 'l')
			conv->modif |= 4;
		else if (*str == 'j')
			conv->modif |= 16;
		else if (*str == 't')
			conv->modif |= 32;
		else if (*str == 'z')
			conv->modif |= 64;
		else
			break ;
		i++;
	}
	return (i);
}

int		resolve_wildcard_or_else(t_conv *conv, char *str, va_list *list,
								int is_precision)
{
	int		d;

	if (*str == '*' || (is_precision && *(str + 1) == '*'))
	{
		d = va_arg(*list, int);
		if (is_precision)
			conv->precision = d < -1 ? -1 : d;
		else
		{
			conv->min_width = d < 0 ? ABS(d) : d;
			conv->pad_dir = d < 0 ? '-' : conv->pad_dir;
		}
		return (0);
	}
	else if (is_precision)
	{
		conv->precision = ft_atoi(str + 1);
		return (ft_nbrlen(conv->precision) == 1 ? 0
												: ft_nbrlen(conv->precision));
	}
	else
	{
		conv->min_width = ft_atoi(str);
		return (ft_nbrlen(conv->min_width) - 1);
	}
}
