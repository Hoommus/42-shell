/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_works.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/14 18:43:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/04/02 16:33:50 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"
#include "shell_script.h"
#include "shell_history.h"

bool			is_printable(const char c[8])
{
	if (c[1] == 0 && ((c[0] >= 10 && c[0] <= 13) || (c[0] >= 32 && c[0] != 127)))
		return (true);
	else if (((c[0] & 0xC0) && (c[1] & 0x80))
		|| ((c[0] & 0xE0) && (c[1] & 0x80) && (c[2] & 0x80))
		|| ((c[0] & 0xF0) && (c[1] & 0x80) && (c[2] & 0x80) && (c[3] & 0x80)))
		return (true);
	else
		return (false);
}

// TODO: Toggle state intelligently if user escapes some special symbol
void			deal_with_printable(const char arr[8])
{
	if (g_term->input_state == STATE_ESCAPED)
		toggle_state("\\");
	else if (!toggle_state(arr) && g_term->input_state == STATE_ESCAPED)
		g_term->input_state = STATE_NORMAL;
	buff_insert_single_at(g_term->buffer->iterator, arr);
	carpos_update(POS_LAST);
	write(1, arr, ft_strlen((char *)arr));
	if (carpos_get(POS_LAST)->col == g_term->ws_col - 1)
	{
		tputs(tgetstr("sf", NULL), 1, &ft_putc);
		caret_move(1, D_RIGHT);
	}
	carpos_update(POS_CURRENT);
	if (carpos_get(POS_LAST)->row == carpos_get(POS_CURRENT)->row
		&& carpos_get(POS_LAST)->col < carpos_get(POS_CURRENT)->col)
		carpos_adjust_db(1);
	if (carpos_get(POS_CURRENT)->col < g_term->ws_col)
		buffer_redraw();
}

void			deal_with_newline(const char arr[8])
{
	if (g_term->input_state == STATE_QUOTE
		|| g_term->input_state == STATE_DQUOTE
		|| g_term->input_state == STATE_ESCAPED)
	{
		write(STDOUT_FILENO, "\n", 1);
		buff_insert_single_at(g_term->buffer->iterator, arr);
		display_prompt(g_term->input_state);
		if (g_term->input_state == STATE_ESCAPED)
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

void			deal_with_sigint(void)
{
	caret_move(g_term->buffer->size - g_term->buffer->iterator, D_RIGHT);
	buff_clear(0);
	ft_printf("\n");
	g_term->input_state = STATE_NORMAL;
	if (g_term->running_process == 0)
		display_normal_prompt();
	context_switch(g_term->context_current);
}

/*
** TODO: Make this one handle long input from Command + V
** TODO: Memory leak at buff_get_part call
*/
char			*read_command(void)
{
	union u_char	input;
	ssize_t			status;

	while (true)
	{
		carpos_update(POS_CURRENT);
		if ((status = read(0, ft_memset(input.arr, 0, 8), 8)) == -1)
		{
			write(1, "\n", 1);
			return (NULL);
		}
		if (input.arr[0] == '\n')
			deal_with_newline(input.arr);
		else if (is_printable(input.arr) && input.arr[0] != '\n')
			deal_with_printable(input.arr);
		else
			handle_key(input);
		if (g_term->input_state == STATE_COMMIT)
			return (history_write(buff_get_part(0, UINT64_MAX), get_history_fd()));
	}
}
