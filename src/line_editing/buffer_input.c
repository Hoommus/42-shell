/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 16:28:45 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/05/14 20:42:08 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"
#include "line_editing.h"

static void			deal_with_printable(const char *arr)
{
	buff_insert_single_at(g_term->buffer->iterator, arr);
	carpos_update(POS_LAST);
	write(1, arr, ft_strlen((char *)arr));
	carpos_update(POS_CURRENT);
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

static void			deal_with_heredoc_nl(const char *arr)
{
	int64_t	offset;
	char	*swap;

	offset = buff_rchr("\n", g_term->buffer->size);
	swap = buff_get_part((u_int64_t)offset + 1, g_term->buffer->size);
	if (swap && ft_strcmp(swap, g_term->heredoc_word) == 0)
	{
		buff_clear((u_int64_t)(offset + 1));
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = STATE_COMMIT;
	}
	else
	{
		handle_end((union u_char){K_END});
		write(STDOUT_FILENO, "\n", 1);
		buff_insert_single_at(g_term->buffer->iterator, arr);
		display_prompt(g_term->input_state);
		buffer_redraw();
	}
	free(swap);
}

static void			deal_with_newline(const char *arr)
{
	if (g_term->input_state == STATE_HEREDOC)
		return (deal_with_heredoc_nl(arr));
	recheck_state(0);
	if (g_term->input_state > STATE_NON_INTERACTIVE)
	{
		handle_end((union u_char){K_END});
		TERM_CLR_LINE;
		write(STDOUT_FILENO, "\n", 1);
		buff_insert_single_at(g_term->buffer->iterator, arr);
		display_prompt(g_term->input_state);
		buffer_redraw();
	}
	else
	{
		handle_end((union u_char){K_END});
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = STATE_COMMIT;
	}
}

static void			deal_with_pasted(char *str)
{
	buff_insert_string_at(g_term->buffer->iterator, str);
	write(1, str, ft_strlen(str));
	if (carpos_update(POS_CURRENT)->col >= g_term->ws_col - 1)
	{
		tputs(tgetstr("sf", NULL), 1, &ft_putc);
		caret_move(1, D_RIGHT);
	}
	recheck_state(0);
}

char				*read_arbitrary(void)
{
	union u_char	input;
	char			swap[1025];

	if (g_term->input_state == STATE_HEREDOC)
	{
		display_prompt(g_term->input_state);
		buff_clear(0);
	}
	while (g_term->input_state != STATE_COMMIT && carpos_update(POS_CURRENT))
	{
		if (read(0, ft_memset(swap, 0, 1025), 1024) == -1)
			return ((char *)(write(1, "\n", 1) & 0));
		else if (is_key_hooked(*(union u_char *)ft_memcpy(input.arr, swap, 8))
			|| input.arr[0] == 0x1b)
			handle_key(input);
		else if (is_single_symbol(swap) && swap[0] == '\n')
			deal_with_newline(swap);
		else if (is_single_symbol(swap) && swap[0] != '\n')
			deal_with_printable(swap);
		else if (!is_single_symbol(swap))
			deal_with_pasted(swap);
	}
	g_term->heredoc_word = NULL;
	return (buff_get_part(0, UINT64_MAX));
}
