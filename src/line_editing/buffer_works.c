#include <twenty_one_sh.h>
#include "twenty_one_sh.h"
#include "line_editing.h"
#include "script_lang.h"
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

/*
** Returns number of 0b10xxxxxx-based characters
*/
u_int64_t		get_utf_body_size(char first)
{
	unsigned char	c;

	c = (unsigned char)first;
	if (c >= 32 && c < 127)
		return (0);
	if ((c & 0xC0) == 0xC0)
		return (1);
	else if ((c & 0xE0) == 0xE0)
		return (2);
	else if ((c & 0xF0) == 0xF0)
		return (3);
	else if (c == 27)
		return (7);
	else
		return (0);
}

void			deal_with_printable(const char arr[8])
{
	if (!toggle_state(arr) && g_term->input_state == STATE_ESCAPED_EOL)
		g_term->input_state = STATE_NORMAL;
	insert_string_at(g_term->v_buffer->iterator, arr);
	save_caret_position_as(POS_LAST);
	tputs(tgetstr("im", NULL), 1, &ft_putc);
	write(1, arr, 8);
	tputs(tgetstr("ei", NULL), 1, &ft_putc);
	if (get_carpos(POS_LAST)->col == g_term->ws_col - 1)
	{
		tputs(tgetstr("sf", NULL), 1, &ft_putc);
		caret_move(1, D_RIGHT);
	}
	update_caret_position(POS_CURRENT);
	if (get_carpos(POS_LAST)->row <= get_carpos(POS_CURRENT)->row
		&& get_carpos(POS_LAST)->col < get_carpos(POS_CURRENT)->col)
		adjust_carpos_db();
	if (get_carpos(POS_CURRENT)->col < g_term->ws_col)
		buffer_redraw(-1);
}

void			deal_with_newline(const char arr[8])
{
	if (g_term->input_state == STATE_QUOTE
		|| g_term->input_state == STATE_DQUOTE
		|| g_term->input_state == STATE_ESCAPED_EOL)
	{
		write(STDOUT_FILENO, "\n", 1);
		insert_string_at(g_term->v_buffer->iterator, arr);
		display_prompt(g_term->input_state);
		toggle_escaped();
		buffer_redraw(-1);
	}
	else
	{
		if (ft_strcmp(buff_char_at(g_term->v_buffer->iterator), "\0") == 0)
			caret_move(g_term->v_buffer->size - g_term->v_buffer->iterator, D_RIGHT);
		write(STDOUT_FILENO, "\n", 1);
		g_term->input_state = STATE_COMMIT;
	}
}

/*
** Using union just to use it anywhere
*/

union			u_char
{
	long		lng;
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
			commands = smart_split(write_history(buff_get_part(0, UINT64_MAX),
						g_term->history_file), TOKEN_DELIMITERS);
		}
		update_caret_position(POS_CURRENT);
	}
	return (commands);
}
