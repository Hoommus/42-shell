/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auxiliary_le.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/22 20:36:27 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/22 20:51:01 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

int		ft_putc(int c)
{
	return ((int)write(2, &c, 1));
}

bool	is_single_symbol(const char *c)
{
	if (c[1] == 0 && ((c[0] >= 10 && c[0] <= 13) || (c[0] >= 32 && c[0] < 127)))
		return (true);
	if (((c[0] & 0xC0) && (c[1] & 0x80) && !c[2]) ||
		((c[0] & 0xE0) && (c[1] & 0x80) && (c[2] & 0x80) && !c[3]) ||
		((c[0] & 0xF0) && (c[1] & 0x80) && (c[2] & 0x80) && (c[3] & 0x80)
																	&& !c[4]))
		return (true);
	else
		return (false);
}

void	write_at(int col, int row, const char *string)
{
	int		i;

	if (g_term->input_state == STATE_NON_INTERACTIVE || g_term->tty_fd == -1 ||
		get_env_v(NULL, "TERM") == NULL)
		return ;
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
