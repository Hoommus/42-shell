/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_anywhere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/16 19:41:32 by vtarasiu          #+#    #+#             */
/*   Updated: 2018/12/18 15:08:14 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

void		write_at(int col, int row, char *string)
{
	int		i;

	carpos_save_as(POS_CUSTOM1);
	tputs(tgoto(tgetstr("cm", NULL), col, row), 1, &ft_putc);
	ft_printf("\x1b[37;1m");
	TERM_CLR_LINE;
	ft_printf("%llu\n", *((long long *)string));
	TERM_CLR_LINE;
	i = 0;
	while (string[i])
		ft_printf("%-2c", string[i++]);
	ft_printf("\n");
	TERM_CLR_LINE;
	i = 0;
	while (string[i])
		ft_printf("%2.2x", string[i++]);
	ft_printf("\n");
	TERM_CLR_LINE;
	ft_printf("\x1b[0m");
	carpos_load(POS_CUSTOM1);
}

