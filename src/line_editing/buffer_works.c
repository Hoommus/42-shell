/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_works.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/14 18:43:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/02/17 14:22:04 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"
#include "shell_script.h"
#include "shell_history.h"

int				is_printable(const char c[8])
{
	if ((c[0] >= 10 && c[0] <= 13) || (c[0] >= 32 && c[0] != 127))
		return (1);
	else if (((c[0] & 0xC0) && (c[1] & 0x80))
		|| ((c[0] & 0xE0) && (c[1] & 0x80) && (c[2] & 0x80))
		|| ((c[0] & 0xF0) && (c[1] & 0x80) && (c[2] & 0x80) && (c[3] & 0x80)))
		return (1);
	else
		return (0);
}

void			deal_with_printable(const char arr[8])
{
	if (!toggle_state(arr) && g_term->input_state == STATE_ESCAPED_EOL)
		g_term->input_state = STATE_NORMAL;
	buff_insert_single_at(g_term->buffer->iterator, arr);
	carpos_update(POS_LAST);
	ft_printf("%s", arr);
	if (carpos_get(POS_LAST)->col == g_term->ws_col - 1)
	{
		tputs(tgetstr("sf", NULL), 1, &ft_putc);
		caret_move(1, D_RIGHT);
	}
	carpos_update(POS_CURRENT);
	if (carpos_get(POS_LAST)->row <= carpos_get(POS_CURRENT)->row
		&& carpos_get(POS_LAST)->col < carpos_get(POS_CURRENT)->col)
		carpus_adjust_db();
	if (carpos_get(POS_CURRENT)->col < g_term->ws_col)
		buffer_redraw();
}

void			deal_with_newline(const char arr[8])
{
	if (g_term->input_state == STATE_QUOTE
		|| g_term->input_state == STATE_DQUOTE
		|| g_term->input_state == STATE_ESCAPED_EOL)
	{
		ft_printf("\n");
		buff_insert_single_at(g_term->buffer->iterator, arr);
		display_prompt(g_term->input_state);
		if (g_term->input_state == STATE_ESCAPED_EOL)
			g_term->input_state = STATE_NORMAL;
		buffer_redraw();
	}
	else
	{
		caret_move(g_term->buffer->size - g_term->buffer->iterator, D_RIGHT);
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = STATE_COMMIT;
	}
}

/*
** TODO: Make this one handle long input from Command + V
** TODO: Memory leak at buff_get_part call
*/
char			**read_command(void)
{
	char			**commands;
	union u_char	input;

	commands = NULL;
	while (!commands)
	{
		read(0, ft_memset(input.arr, 0, 8), 8);
		if (input.arr[0] == '\n')
			deal_with_newline(input.arr);
		else if (is_printable(input.arr) && input.arr[0] != '\n')
			deal_with_printable(input.arr);
		else
			handle_key(input);
		if (g_term->input_state == STATE_COMMIT)
		{
			// TODO: Fix leak from buff_get_part
			commands = smart_split(history_write(buff_get_part(0, UINT64_MAX),
										get_history_fd()), TOKEN_DELIMITERS);
		}
		if (input.lng != 0)
			carpos_update(POS_CURRENT);
	}
	return (commands);
}
