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
	insert_single_at(g_term->buffer->iterator, arr);
	carpos_save_as(POS_LAST);
	tputs(tgetstr("im", NULL), 1, &ft_putc);
	write(1, arr, 8);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	if (carpos_get(POS_LAST)->col == g_term->ws_col - 1)
	{
		tputs(tgetstr("sf", NULL), 1, &ft_putc);
		caret_move(1, D_RIGHT);
	}
	carpos_update(POS_CURRENT);
	if (carpos_get(POS_LAST)->row <= carpos_get(POS_CURRENT)->row
		&& carpos_get(POS_LAST)->col < carpos_get(POS_CURRENT)->col)
		adjust_carpos_db();
	if (carpos_get(POS_CURRENT)->col < g_term->ws_col)
		buffer_redraw(-1);
}

void			deal_with_newline(const char arr[8])
{
	if (g_term->input_state == STATE_QUOTE
		|| g_term->input_state == STATE_DQUOTE
		|| g_term->input_state == STATE_ESCAPED_EOL)
	{
		write(STDOUT_FILENO, "\n", 1);
		insert_single_at(g_term->buffer->iterator, arr);
		display_prompt(g_term->input_state);
		toggle_escaped();
		buffer_redraw(-1);
	}
	else
	{
		if (ft_strcmp(buff_char_at(g_term->buffer->iterator), "\0") == 0)
			caret_move(g_term->buffer->size - g_term->buffer->iterator, D_RIGHT);
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = STATE_COMMIT;
	}
}

/*
** Using union just to use it anywhere
*/

union			u_char
{
	u_int64_t	lng;
	char		arr[8];
};

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
			handle_key(input.lng);
		if (g_term->input_state == STATE_COMMIT)
		{
			// TODO: Fix leak from buff_get_part
			commands = smart_split(history_write(buff_get_part(0, UINT64_MAX),
										get_history_fd()), TOKEN_DELIMITERS);
		}
		carpos_update(POS_CURRENT);
	}
	return (commands);
}
